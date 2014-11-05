from subprocess import call

def buildAll(projects):
  for project in projects:
      base = "./"+project+"/"
      print base
      call("make", "-C", base)
      #call(["cp", base+"bin/"+project, "./bin"])
      
executables=["child_one","child_two","parent"]
buildAll(executables)
