
""" define various two color cellular automata rules.
    p = value of cell above and the the left
    q = value of cell immediately above
    r = value of cell above and to right

"""


def rule_22(p:int, q:int, r:int):
    return (p + q + r + p*q*r) % 2 

def rule_30(p:int, q:int, r:int):
    return (p + q + r + q*r) % 2 

def rule_110(p:int, q:int, r:int):
    return (q + r + q*r + p*q*r) % 2

def rule_45(p:int, q:int, r:int):
    return (1 + p + r + q*r) % 2 

def rule_60(p:int, q:int, r:int):

    return (p+q) % 2 

def rule_105(p:int, q:int, r:int):
    return (1 + p + q + r) % 2

def rule_225(p:int, q:int, r:int):
    return (1 + p + q + r + q*r) % 2

def rule_73(p:int, q:int, r:int):
    return (1 + p + q + r + p*r + p*q*r) % 2 
            