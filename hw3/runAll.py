from subprocess import call

def printAll(filenames):
  for x in filenames:
    call(["./pager",x, "100000"])
    
printAll( ["VRUPL_Logo.raw", "LUG_Newbies.raw", "Esher.raw", "calibration.raw", "diver.raw", "JohnBell.raw", "diver_resize.raw", "light_drops.raw", "GrandeJatte.raw"])