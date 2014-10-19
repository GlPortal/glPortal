import bpy
from bpy.props import *

types = [
    ("none" ,"None" , "No special property"),
    ("wall" ,"Wall" , "Wall"),     
    ("trigger" , "Trigger" , "trigger")
]

triggerTypes = [
    ("none" ,"None" , "No special property"), 
    ("win" , "Trigger Win" , "Area triggers win"), 
    ("death" , "Trigger Death" , "Area triggers death"),
    ("radiation" , "Trigger Radiation" , "Area triggers rad"),
    ("portable" , "Portable" , "Set Wall Portable")
]

def setTypes():
    bpy.types.Object.types = EnumProperty(
        items = types,
        name = "type")

def setTriggerTypes():
    bpy.types.Object.triggerTypes = EnumProperty(
        items = triggerTypes,
        name = "triggerType")    
    
setTypes()
setTriggerTypes();

class GlPortalObjectPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    def draw(self, context):
        object = context.active_object
        layout = self.layout
        row = layout.row()

        layout.prop(object, "types")
        if object.types == "trigger":
            layout.prop(object, "triggerTypes")
            
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.save_object_type", text="save", icon='MESH_CUBE')
        col.operator("wm.clear_selection", text="Clear Type", icon='MESH_CUBE')
