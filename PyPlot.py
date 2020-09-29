"""
Created on Fri Jul 10 05:03:41 2020

@author: Xiwei Wang
"""

import numpy as np
import matplotlib.pyplot as plt
import sys

sys.argv=['PyPlot.py']

def PassArg(xs, ys, x, y):
    if len(xs) == 2 and len(ys) == 2:
        xs[0]=xs[1]
        ys[0]=ys[1]
        xs[1]=x
        ys[1]=y
        return True
    else:
        xs.append(x)
        ys.append(y)
        return False

def PlotTest():
    print("Print test figure")
    plt.figure()
    plt.plot([1,2,3], [3,6,9])
    plt.show()  

def PolyPlot(param):
    plt.figure()
    plt.plot(param[0], param[1])
    plt.show()
def PrettyPolyPlot(param):
    plt.ion()
    xs=[]
    ys=[]
    for i in range(len(param[0])):
        if PassArg(xs, ys, param[0][i], param[1][i]) == False:
            pass
        plt.plot(xs, ys)
        plt.pause(0.1)


















