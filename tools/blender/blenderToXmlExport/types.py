import bpy
from bpy.props import *

glpTypes = [
    ("none" , "None", "No special property"),
    ("wall" , "Wall", "Wall"),
    ("door" , "Door", "Door"),
    ("volume", "Volume", "Volume"),
    ("trigger", "Trigger", "trigger")
]

glpWallTypes = [
    ("default" ,"Default" , "Nothing special just a painted concrete wall"),
    ("invisible" ,"Invisible" , "Invisible (Not implemented)"),    
    ("glass" ,"Glass" , "Glass (Not implemented)"),
    ("portable" ,"Portable" , "Portable Wall")
]

glpVolumeTypes = [
    ("acid", "Acid Pool", "A pool full of acid, hurts..")
]

glpTriggerTypes = [
    ("none" ,"None" , "No special property"), 
    ("win" , "Trigger Win" , "Area triggers win"), 
    ("death" , "Trigger Death" , "Area triggers death (Not implemented)"),
    ("radiation" , "Trigger Radiation" , "Area triggers rad (Not implemented)")
]

def setTypes():
    bpy.types.Object.glpTypes = EnumProperty(
        items = glpTypes,
        name = "Type"
    )

def setVolumeTypes():
    bpy.types.Object.glpVolumeTypes = EnumProperty(
        items = glpVolumeTypes,
        name = "Volume Type"
    )

def setTriggerTypes():
    bpy.types.Object.glpTriggerTypes = EnumProperty(
        items = glpTriggerTypes,
        name = "Trigger Type"
    )    

def setWallTypes():
    bpy.types.Object.glpWallTypes = EnumProperty(
        items = glpWallTypes,
        name = "Wall Type"
    )    

setTypes()
setVolumeTypes()
setTriggerTypes()
setWallTypes()
