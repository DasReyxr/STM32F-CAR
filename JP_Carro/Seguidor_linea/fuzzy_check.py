import numpy as np
import skfuzzy as fuzz

# Sensor membership parameters from FuzzyLogic.c
BLACK_ARGS = (350.0, 0.0, 500.0)
GRAY_ARGS = (400.0, 500.0, 600.0)
WHITE_ARGS = (550.0, 0.0, 600.0)

# Output membership parameters from FuzzyLogic.h
LOW_ARGS = (0.0, 0.0, 50.0)
MID_ARGS = (25.0, 50.0, 75.0)
HIGH_ARGS = (50.0, 0.0, 100.0)

OUTPUT_UNIVERSE = np.linspace(0, 100, 1001)

# Centroide constants from Centroide.c (recomputed at startup)
Y1 = None
X1 = None
Y2 = None
X2 = None
TLeft = None
TT = None
TRight = None
C_TLeft = None
C_TT = None
C_TRight = None


# --- low/high trap and triangular membership ---

def trapz_low(x, L, R):
    if x <= L:
        return 1.0
    if x < R:
        return (R - x) / (R - L)
    return 0.0


def trapz_high(x, L, R):
    if x <= L:
        return 0.0
    if x < R:
        return (x - L) / (R - L)
    return 1.0


def triang(x, L, C, R):
    if x <= L:
        return 0.0
    if x < C:
        return (x - L) / (C - L)
    if x < R:
        return (R - x) / (R - C)
    return 0.0


def black_membership(x):
    return trapz_low(x, BLACK_ARGS[0], BLACK_ARGS[2])


def gray_membership(x):
    return triang(x, *GRAY_ARGS)


def white_membership(x):
    return trapz_high(x, WHITE_ARGS[0], WHITE_ARGS[2])


# --- output fuzzy membership definitions ---

def output_low(x):
    return fuzz.trapmf(x, [LOW_ARGS[0], LOW_ARGS[1], LOW_ARGS[2], LOW_ARGS[2]])


def output_mid(x):
    return fuzz.trimf(x, [MID_ARGS[0], MID_ARGS[1], MID_ARGS[2]])


def output_high(x):
    return fuzz.trapmf(x, [HIGH_ARGS[0], HIGH_ARGS[0], HIGH_ARGS[2], HIGH_ARGS[2]])


# --- centroid support ---

def Centroid_T(x0, x1, x2):
    return (x0 + x1 + x2) / 3.0


def Centroid_T_R(x0, x1):
    return x0 + ((x1 - x0) * (2.0 / 3.0))


def Centroid_Sq(x0, x1):
    return (x0 + x1) / 2.0


def f_low(y):
    return (LOW_ARGS[2] - y) / (-1.0 / (LOW_ARGS[2] - LOW_ARGS[0]))


def f_Mid1(y):
    return (y - MID_ARGS[0]) / (1.0 / (MID_ARGS[1] - MID_ARGS[0]))


def f_Mid2(y):
    return (MID_ARGS[2] - y) / (-1.0 / (MID_ARGS[2] - MID_ARGS[1]))


def f_High(y):
    return (y - HIGH_ARGS[0]) / (1.0 / (HIGH_ARGS[2] - HIGH_ARGS[0]))


# --- C translation of Centroide ---

def centroide_init():
    global Y1, X1, Y2, X2, TLeft, TT, TRight, C_TLeft, C_TT, C_TRight
    Y1 = (LOW_ARGS[2] * MID_ARGS[1] - LOW_ARGS[0] * MID_ARGS[0]) / (
        (MID_ARGS[1] - MID_ARGS[0]) + (LOW_ARGS[2] - LOW_ARGS[0]))
    X1 = f_Mid1(Y1)

    Y2 = (HIGH_ARGS[2] * MID_ARGS[2] - HIGH_ARGS[0] * MID_ARGS[1]) / (
        (MID_ARGS[2] - MID_ARGS[1]) + (HIGH_ARGS[2] - HIGH_ARGS[0]))
    X2 = f_High(Y2)

    TLeft = Y1 * (LOW_ARGS[2] - MID_ARGS[0]) / 2.0
    TT = (MID_ARGS[2] - MID_ARGS[0]) / 2.0
    TRight = (MID_ARGS[2] - HIGH_ARGS[0]) * Y2 / 2.0

    C_TLeft = Centroid_T(LOW_ARGS[2], MID_ARGS[0], X1)
    C_TT = Centroid_T(MID_ARGS[0], MID_ARGS[2], MID_ARGS[1])
    C_TRight = Centroid_T(MID_ARGS[2], HIGH_ARGS[0], X2)


def Area_Low(y_low):
    x_low = f_low(y_low)
    A_Sq_low = y_low * x_low
    A_T_low = y_low * (LOW_ARGS[2] - x_low) / 2.0
    C_Sq_low = Centroid_Sq(0, x_low)
    C_T_low = Centroid_T_R(x_low, LOW_ARGS[2])
    return {
        'num': A_Sq_low * C_Sq_low + A_T_low * C_T_low,
        'den': A_Sq_low + A_T_low,
    }


def Area_Mid(y_mid):
    x_mid0 = f_Mid1(y_mid)
    x_mid1 = f_Mid2(y_mid)
    A_TS = (x_mid1 - x_mid0) * (1 - y_mid) / 2.0
    A_TT = (MID_ARGS[2] - MID_ARGS[0]) / 2.0
    C_TS = Centroid_T(x_mid0, x_mid1, MID_ARGS[1])
    C_TT_ = Centroid_T(MID_ARGS[0], MID_ARGS[2], MID_ARGS[1])
    return {
        'num': A_TT * C_TT_ - A_TS * C_TS,
        'den': A_TT - A_TS,
    }


def Area_High(y_high):
    x_high = f_High(y_high)
    A_Sq_high = (100.0 - x_high) * y_high
    A_T_high = (x_high - HIGH_ARGS[0]) * y_high / 2.0
    C_Sq_high = Centroid_Sq(x_high, 100.0)
    C_T_high = Centroid_T_R(HIGH_ARGS[0], x_high)
    return {
        'num': A_Sq_high * C_Sq_high + A_T_high * C_T_high,
        'den': A_Sq_high + A_T_high,
    }


def Area_InterLm(y_low, y_mid):
    y_lm = min(y_low, y_mid)
    x_lm0 = f_Mid1(y_lm)
    x_lm1 = f_low(y_lm)
    A_T1 = (x_lm1 - x_lm0) * (Y1 - y_lm) / 2.0 if y_lm < Y1 else 0.0
    return {
        'num': TLeft * C_TLeft - A_T1 * Centroid_T(x_lm0, x_lm1, X1),
        'den': TLeft - A_T1,
    }


def Area_InterMh(y_mid, y_high):
    y_mh = min(y_mid, y_high)
    x_mh0 = f_High(y_mh)
    x_mh1 = f_Mid2(y_mh)
    T2 = (x_mh1 - x_mh0) * (Y2 - y_mh) / 2.0 if y_mh < Y1 else 0.0
    TRight_local = (MID_ARGS[2] - HIGH_ARGS[0] * Y2) / 2.0
    return {
        'num': TRight_local * C_TRight - T2 * Centroid_T(x_mh0, x_mh1, X2),
        'den': TRight_local - T2,
    }


def centroide_c(y_low, y_mid, y_high):
    A_L = Area_Low(y_low)
    A_M = Area_Mid(y_mid)
    A_H = Area_High(y_high)
    A_LM = Area_InterLm(y_low, y_mid)
    A_MH = Area_InterMh(y_mid, y_high)
    num = A_L['num'] + A_M['num'] + A_H['num'] + A_LM['num'] + A_MH['num']
    den = A_L['den'] + A_M['den'] + A_H['den'] + A_LM['den'] + A_MH['den']
    return 0.0 if den == 0 else num / den


# --- inference and defuzz ---

def mamdani_rule_strength(a, b):
    return min(a, b)


def sensor_membership(set_name, raw):
    if set_name == 'black':
        return black_membership(raw)
    if set_name == 'gray':
        return gray_membership(raw)
    if set_name == 'white':
        return white_membership(raw)
    raise ValueError(set_name)


RULES = [
    ('white', 'white', 'mid', 'mid'),
    ('white', 'gray', 'mid', 'low'),
    ('gray', 'white', 'low', 'mid'),
    ('gray', 'black', 'low', 'high'),
    ('black', 'gray', 'high', 'low'),
    ('black', 'black', 'low', 'low'),
    ('gray', 'gray', 'mid', 'mid'),
    ('white', 'black', 'low', 'high'),
    ('black', 'white', 'high', 'low'),
]


def mamdani_infer(left_raw, right_raw):
    a_mi = {'low': 0.0, 'mid': 0.0, 'high': 0.0}
    a_md = {'low': 0.0, 'mid': 0.0, 'high': 0.0}
    for left_set, right_set, out_left, out_right in RULES:
        ml = sensor_membership(left_set, left_raw)
        mr = sensor_membership(right_set, right_raw)
        strength = mamdani_rule_strength(ml, mr)
        a_mi[out_left] = max(a_mi[out_left], strength)
        a_md[out_right] = max(a_md[out_right], strength)
    return a_mi, a_md


def fuzzy_output_aggregate(a_low, a_mid, a_high):
    low_mf = np.minimum(a_low, output_low(OUTPUT_UNIVERSE))
    mid_mf = np.minimum(a_mid, output_mid(OUTPUT_UNIVERSE))
    high_mf = np.minimum(a_high, output_high(OUTPUT_UNIVERSE))
    return np.fmax(low_mf, np.fmax(mid_mf, high_mf))


def output_centroid_expected(a_low, a_mid, a_high):
    agg = fuzzy_output_aggregate(a_low, a_mid, a_high)
    return fuzz.defuzz(OUTPUT_UNIVERSE, agg, 'centroid')


def evaluate(left_raw, right_raw):
    a_mi, a_md = mamdani_infer(left_raw, right_raw)
    left_c = centroide_c(a_mi['low'], a_mi['mid'], a_mi['high'])
    right_c = centroide_c(a_md['low'], a_md['mid'], a_md['high'])
    left_expected = output_centroid_expected(a_mi['low'], a_mi['mid'], a_mi['high'])
    right_expected = output_centroid_expected(a_md['low'], a_md['mid'], a_md['high'])
    return {
        'left_raw': left_raw,
        'right_raw': right_raw,
        'activation_left': a_mi,
        'activation_right': a_md,
        'left_centroid_c': left_c,
        'right_centroid_c': right_c,
        'left_centroid_expected': left_expected,
        'right_centroid_expected': right_expected,
    }


if __name__ == '__main__':
    centroide_init()
    print('Ingrese los valores crudos del sensor izquierdo y derecho separados por un espacio.')
    print('Escriba q o enter para salir.')

    while True:
        user_input = input('left right> ').strip()
        if not user_input or user_input.lower() in ('q', 'quit', 'exit'):
            break

        try:
            left_raw, right_raw = map(float, user_input.split())
        except ValueError:
            print('Entrada inválida. Ingrese dos números separados por espacio.')
            continue

        result = evaluate(left_raw, right_raw)
        print(f"{result['left_centroid_expected']:.4f} {result['right_centroid_expected']:.4f}")
