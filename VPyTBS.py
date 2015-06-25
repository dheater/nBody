#Set-up
from __future__ import division
from visual import *
from visual.filedialog import get_file
from visual.graph import *
from unicodedata import *

scene = display(title = "SPICE - TBS Visual", x = 960, y =0,
                width = 950, height = 550, background = color.black)
scene.range = (1.4e7,1.4e7,1.4e7) #1.4e7

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 933333, material = materials.earth)
sat = sphere(pos = (7e6, 0, 0), radius =373333, material = materials.rough, color = color.white)
uranus = sphere(pos = (-7e6, 0, 0), radius = 483333, material = materials.wood, color = color.cyan)

sat.trail = curve(color = color.orange)
earth.trail = curve(color = color.blue)
uranus.trail = curve(color = color.red)

orbitCount = label(box = false, pos = sat.pos + (0, 1e6, 0), text = "Orbits: 0\nVel (m/s): 0", color=color.white)

egraph = gdisplay(x = 10, y = 0, width = 950, height = 550, title = 'TBS Dynamic Energy Graph',
			     xtitle = 'Time(tick)', ytitle = 'Energy(J)', foreground = color.black, background = color.white)
percerrgraph = gdisplay(x = 10, y = 588, width = 950, height = 400, title = 'TBS % Error Graph',
			     xtitle = 'Time(tick)', ytitle = '% Error Net Energy(J)', foreground = color.black, background = color.white)
efunction = gcurve(gdisplay = egraph, color = color.red)
kfunction = gcurve(gdisplay = egraph, color = color.green)
ufunction = gcurve(gdisplay = egraph, color = color.blue)
percerrfunc = gcurve(gdisplay = percerrgraph, color = color.magenta)

GM = (-6.67384e-11*5.972e24)
sat_mass = 5000

earth.vel = vector(0, 0, 0)
sat.vel = vector(0, 0, 0)
uranus.vel = vector(0, 0, 0)

ediff = 0
percerror = 0
first = true

t = 0.1
dt = 0.2

pastx = sat.pos.x
pasty = sat.pos.y

T = 5828.2
Ct = 0
orbits = 0.0
torb = 0.0

#Open satPos.txt (output file for TBOS.c)
with open('SystemStates.txt', 'r') as fd:
    for line in fd:
	#Reads in the position vectors for bodies
		#Earth
	earth.pos.x = (longdouble(line.split()[0]))
	earth.pos.y = (longdouble(line.split()[1]))
	earth.pos.z = (longdouble(line.split()[2]))
	
	earth.vel.x = (longdouble(line.split()[3]))
	earth.vel.y = (longdouble(line.split()[4]))
	earth.vel.z = (longdouble(line.split()[5]))
		#Sat
	sat.pos.x = (longdouble(line.split()[6]))
	sat.pos.y = (longdouble(line.split()[7]))
	sat.pos.z = (longdouble(line.split()[8]))
	
	orbitCount.pos = sat.pos + (0, 1e6, 0)
	
	sat.vel.x = (longdouble(line.split()[9]))
	sat.vel.y = (longdouble(line.split()[10]))
	sat.vel.z = (longdouble(line.split()[11]))
	
		#Uranus
	uranus.pos.x = (longdouble(line.split()[12]))
	uranus.pos.y = (longdouble(line.split()[13]))
	uranus.pos.z = (longdouble(line.split()[14]))
	
	uranus.vel.x = (longdouble(line.split()[15]))
	uranus.vel.y = (longdouble(line.split()[16]))
	uranus.vel.z = (longdouble(line.split()[17]))
	
		#Orbital Time
	Ct = longdouble(line.split()[18])
	
	#Graphing
	ufunction.plot(pos = (t, (GM*sat_mass)/(mag(sat.pos-earth.pos))))
	kfunction.plot(pos = (t, (0.5*sat_mass*(mag(sat.vel)**2))))
	efunction.plot(pos = (t, (GM*sat_mass)/(mag(sat.pos-earth.pos)) + ((0.5*sat_mass)*(mag(sat.vel)**2))))
	
	if(first):
		ediff = ((GM*sat_mass)/(mag(sat.pos-earth.pos)) + ((0.5*sat_mass)*(mag(sat.vel)**2)))
		first = false
	
	percerrfunc.plot(pos = (t,abs((((GM*sat_mass)/(mag(sat.pos-earth.pos)) + ((0.5*sat_mass)*(mag(sat.vel)**2))-ediff)/ediff)*100.0)))
	
	
	t += dt
	
	sat.trail.append(pos=sat.pos)
	earth.trail.append(pos=earth.pos)
	uranus.trail.append(pos=uranus.pos)
	
	Torb = Ct/T
	
	orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"
	
	#Q1
	if sat.pos.y>0 and sat.pos.x>0:
		orbits = arcsin(sat.pos.y/mag((sat.pos.x, sat.pos.y, 0)))/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"
	#Q2
	elif sat.pos.y>0 and sat.pos.x<0:
		orbits = arccos(sat.pos.x/mag((sat.pos.x, sat.pos.y, 0)))/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"
	#Q3
	elif sat.pos.y<0 and sat.pos.x<0:
		orbits = (-arccos(sat.pos.x/mag((sat.pos.x, sat.pos.y, 0)))+2*pi)/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"
	#Q4
	elif sat.pos.y<0 and sat.pos.x>0:
		orbits = (arcsin(sat.pos.y/mag((sat.pos.x, sat.pos.y, 0)))+2*pi)/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"

	# Questionable Code
	elif sat.pos.y==0 and sat.pos.x>0:
		orbits = arcsin(sat.pos.y/mag((sat.pos.x, sat.pos.y, 0)))/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"

	elif sat.pos.y==0 and sat.pos.x<0:
		orbits = (arcsin(sat.pos.y/mag((sat.pos.x, sat.pos.y, 0)))+pi)/(2*pi)
		orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(sat.vel))) + " m/s"

	
		
	#Explosion
	if mag(earth.pos-sat.pos) <= earth.radius+sat.radius:
		earth.color=color.red
		sat.color=color.red
		print '\a'
		break
	if mag(earth.pos-uranus.pos) <= earth.radius+uranus.radius:
		earth.color=color.red
		uranus.color=color.red
		print '\a'
		break
	if mag(uranus.pos-sat.pos) <= uranus.radius+sat.radius:
		uranus.color=color.red
		sat.color=color.red
		print '\a'
		break
	
	scene.center=(sat.pos+earth.pos+uranus.pos)/3
	#scene.center = sat.pos
	#scene.center = earth.pos

fd.close()
