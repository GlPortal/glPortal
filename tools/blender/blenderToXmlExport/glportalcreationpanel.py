import bpy

class GlPortalCreationPanel(bpy.types.Panel):
    """GlPortal creation panel in the toolbar"""
    bl_label = "GlPortal"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    bl_context = "objectmode"
    bl_category = "Create"  

    def draw(self, context):
        layout = self.layout
        layout.label("Objects")
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.add_door", text="Door", icon='MESH_CUBE')

        layout.label("Walls")
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.add_portable", text="Add Portable", icon='MESH_PLANE')
        col.operator("wm.add_wall", text="Add Tiles", icon='MESH_PLANE')
        
        layout.label("Triggers")
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.add_death", text="Death", icon='MESH_CUBE')
        col.operator("wm.add_radiation", text="Radiation", icon='MESH_CUBE')
        col.operator("wm.add_win", text="Win", icon='MESH_CUBE')
