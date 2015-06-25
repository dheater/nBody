#Set-up
from __future__ import division
from visual import *
from visual.filedialog import get_file
from visual.graph import *
from unicodedata import *

scene = display(title = "SPICE - TBS Visual", x = 960, y =0,
                width = 950, height = 550, background = color.black)
scene.range = (1.4e7,1.4e7,1.4e7)

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 933333, material = materials.earth)
moon = sphere(pos = (7e6, 0, 0), radius =373333, material = materials.rough, color = color.white)
moon.trail = curve(color = color.orange)
earth.trail = curve(color = color.blue)
orbitCount = label(box = false, pos = moon.pos + (0, 1e6, 0), text = "Orbits: 0\nVel (m/s): 0", color=color.white)

egraph = gdisplay(x = 10, y = 0, width = 950, height = 550, title = 'TBS Dynamic Energy Graph',
			     xtitle = 'Time(tick)', ytitle = 'Energy(J)', foreground = color.black, background = color.white)
percerrgraph = gdisplay(x = 10, y = 588, width = 950, height = 400, title = 'TBS % Error Graph',
			     xtitle = 'Time(tick)', ytitle = '% Error Net Energy(J)', foreground = color.black, background = color.white)
efunction = gcurve(gdisplay = egraph, color = color.red)
kfunction = gcurve(gdisplay = egraph, color = color.green)
ufunction = gcurve(gdisplay = egraph, color = color.blue)
percerrfunc = gcurve(gdisplay = percerrgraph, color = color.magenta)

GM = (-6.67384e-11*5.972e24)
moon_mass = 5000
moon_rad = 373333
earth_rad = 933333

moon.vel = vector(0, 0, 0)

ediff = 0
percerror = 0
first = true

t = 0.1
dt = 0.2

pastx = moon.pos.x
pasty = moon.pos.y

T = 5828.2
Ct = 0
orbits = 0.0
torb = 0.0

#Open MoonPos.txt (output file for TBOS.c)
with open('SystemStates.txt', 'r') as fd:
    for line in fd:
	#Reads in the position vectors for earth/moon
	moon.pos.x = (longdouble(line.split()[0]))
	moon.pos.y = (longdouble(line.split()[1]))
	moon.pos.z = (longdouble(line.split()[2]))
	orbitCount.pos = moon.pos + (0, 1e6, 0)
	
	earth.pos.x = (longdouble(line.split()[3]))
	earth.pos.y = (longdouble(line.split()[4]))
	earth.pos.z = (longdouble(line.split()[5]))
	
	moon.vel.x = (longdouble(line.split()[6]))
	moon.vel.y = (longdouble(line.split()[7]))
	moon.vel.z = (longdouble(line.split()[8]))
        
        Ct = longdouble(line.split()[9])
	
	ufunction.plot(pos = (t, (GM*moon_mass)/(mag(moon.pos-earth.pos))))
	kfunction.plot(pos = (t, (0.5*moon_mass*(mag(moon.vel)**2))))
	efunction.plot(pos = (t, (GM*moon_mass)/(mag(moon.pos-earth.pos)) + ((0.5*moon_mass)*(mag(moon.vel)**2))))
	
	if(first):
		ediff = ((GM*moon_mass)/(mag(moon.pos-earth.pos)) + ((0.5*moon_mass)*(mag(moon.vel)**2)))
		first = false
	
	percerrfunc.plot(pos = (t,abs((((GM*moon_mass)/(mag(moon.pos-earth.pos)) + ((0.5*moon_mass)*(mag(moon.vel)**2))-ediff)/ediff)*100.0)))
	
	
	t += dt
	
	moon.trail.append(pos=moon.pos)
	earth.trail.append(pos=earth.pos)
	
        Torb = Ct/T
        
	orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"
	
	#Q1
	if moon.pos.y>0 and moon.pos.x>0:
		orbits = arcsin(moon.pos.y/mag((moon.pos.x, moon.pos.y, 0)))/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"
	#Q2
	elif moon.pos.y>0 and moon.pos.x<0:
		orbits = arccos(moon.pos.x/mag((moon.pos.x, moon.pos.y, 0)))/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"
	#Q3
	elif moon.pos.y<0 and moon.pos.x<0:
		orbits = (-arccos(moon.pos.x/mag((moon.pos.x, moon.pos.y, 0)))+2*pi)/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"
	#Q4
	elif moon.pos.y<0 and moon.pos.x>0:
		orbits = (arcsin(moon.pos.y/mag((moon.pos.x, moon.pos.y, 0)))+2*pi)/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"

	# Questionable Code
	elif moon.pos.y==0 and moon.pos.x>0:
		orbits = arcsin(moon.pos.y/mag((moon.pos.x, moon.pos.y, 0)))/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"

	elif moon.pos.y==0 and moon.pos.x<0:
		orbits = (arcsin(moon.pos.y/mag((moon.pos.x, moon.pos.y, 0)))+pi)/(2*pi)
                orbitCount.text = "Orb(rad): " + str(double(int(orbits*1000))/1000) + "  Orb(t): " + str(double(int(Torb*1000))/1000) +  "\nVel (m/s): " + str(int(mag(moon.vel))) + " m/s"

	
		
	#Explosion
	if mag(earth.pos-moon.pos) <= earth.radius+moon.radius:
		earth.color=color.red
		moon.color=color.red
		print '\a'
		break
	
	#scene.center=(moon.pos+earth.pos)/2
	#scene.center = moon.pos
	scene.center = earth.pos

#t = 0

#expl = text(text='Boom!!!', pos=((moon.pos-earth.pos)/mag(moon.pos-earth.pos))*moon.radius, height = 2e9, color = color.orange, depth = 2e8)

#while true:
#	t+=1
#	if t<2000:
#		expl.radius=expl.radius*1.1
#	else:
#		expl.radius=expl.radius*1.001
#	rate(1000)
	


fd.close()
