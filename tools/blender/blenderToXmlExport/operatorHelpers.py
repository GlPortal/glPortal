import bpy

def clearGlpProperties():
    object = bpy.context.active_object
    if object:
        object.glpTypes = "none"
        object.glpTriggerTypes = "none"
        object.glpWallTypes = "none"        
