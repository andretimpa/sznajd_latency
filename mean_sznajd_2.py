#! /usr/bin/env python

# Integrates the mean field equations for the Sznajd model with latency and 2 opinions

# a notacao da saida eh: t - eta - nu

import numpy
import scipy
import scipy.integrate as integrate
import math


def distance(x, y):
    dist = 0
    for i in range(x.size):
        if math.fabs(x[i] - y[i]) > dist:
            dist = math.fabs(x[i] - y[i])
    return dist


resolution  = 0.1
MC_steps    = 1000
trimm_res   = 0.002
M = 4
m = M/2
unitary = numpy.ones(m)
D    = numpy.linspace(0, 0.9666, num = 30)
lamb = numpy.linspace(0.01, 0.26, num = 26)
points = [(x,y) for x in D for y in lamb]


#a funcao que sera usada para d(eta)/dt = F
#eta = [e, n]
def F(eta, t):
    global m, lamb
    return numpy.array([-lamb*eta[0] + eta[3]*(eta[0] + eta[2])**2, -lamb*eta[1] + eta[2]*(eta[1] + eta[3])**2, lamb*eta[0] - eta[2]*(eta[1] + eta[3])**2, lamb*eta[1] - eta[3]*(eta[0] + eta[2])**2]) #M=2
    #return numpy.concatenate(( -lamb*eta[:m] + ((eta[:m] + eta[m:])**2)*(numpy.dot(eta[m:], unitary) - eta[m:]), lamb*eta[:m] - eta[m:]*(numpy.dot((eta[:m] + eta[m:])**2, unitary) - (eta[:m] + eta[m:])**2) ))   #M geral


sim = 0
time_points = numpy.arange(0, MC_steps + resolution, resolution, float)
for D, lamb in points:
    sim += 1
    time_points_cat = numpy.array([], float).reshape((0, ))
    trimmed         = numpy.array([], float).reshape((0, M))
    out_name        = "out_" + str(sim) + ".dat"
    point           = numpy.array([0.5*(D+1), 0.5*(1-D), 0, 0], dtype = float)
    data_out        = integrate.odeint(F, point, time_points).reshape(time_points.size, M)
    write_flag      = True
    last_outputed   = data_out[0]
    pos_aux         = 0
    for i in range(1, time_points.size):
        if distance(last_outputed, data_out[i]) > trimm_res:
            last_outputed = data_out[i]
            if write_flag == False:
                write_flag = True
                pos_aux    = i
        else:
            if write_flag == True:
                write_flag       = False
                trimmed          = numpy.concatenate((trimmed, data_out[pos_aux:i]))
                time_points_cat  = numpy.concatenate((time_points_cat, time_points[pos_aux:i]))
    if write_flag == False:
        trimmed          = numpy.concatenate((trimmed, data_out[time_points.size - 1 : time_points.size]))
        time_points_cat  = numpy.concatenate((time_points_cat, time_points[time_points.size - 1 : time_points.size]))
    else:
        trimmed          = numpy.concatenate((trimmed, data_out[pos_aux : time_points.size]))
        time_points_cat  = numpy.concatenate((time_points_cat, time_points[pos_aux : time_points.size]))
    data_out = numpy.concatenate((time_points_cat.reshape((time_points_cat.size, 1)), trimmed), axis=1)
    output_file = open(out_name, "w")
    output_file.write(str(D) + "\t" + str(lamb) + "\n")
    data_out.tofile(output_file, " ")
    output_file.close()

