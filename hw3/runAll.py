from subprocess import call

print "PictureName,CacheSize,Hit %, Miss %, AVG Times Swapped,CacheType"

def printAll(filenames):
  for x in filenames:
    with open("./csv/" + x + ".csv", "w") as outfile:
      #call(["./pager",x], stdout=outfile)
      call(["./pager",x,"10000000",str(256*1)])
      call(["./pager",x,"10000000",str(256*3)])
      call(["./pager",x,"10000000",str(256*6)])
      call(["./pager",x,"10000000",str(256*12)])
      call(["./pager",x,"10000000",str(256*25)])
      call(["./pager",x,"10000000",str(256*50)])
      call(["./pager",x,"10000000",str(256*100)])
    
#printAll( ["VRUPL_Logo.raw", "LUG_Newbies.raw", "Esher.raw", "calibration.raw", "diver.raw", "JohnBell.raw", "diver_resize.raw", "light_drops.raw", "GrandeJatte.raw"])
printAll( ["VRUPL_Logo.raw", "LUG_Newbies.raw", "light_drops.raw", "GrandeJatte.raw"])