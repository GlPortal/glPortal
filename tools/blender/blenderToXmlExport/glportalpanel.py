import bpy

class GlPortalPanel(bpy.types.Panel):
    """GlPortal panel in the toolbar"""
    bl_label = "GlPortal Quick Bar"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'

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
        col.operator("wm.add_portable", text="Portable", icon='MESH_PLANE')
        col.operator("wm.add_wall", text="Default", icon='MESH_PLANE')

        
        layout.label("Triggers")
        row = layout.row()
        split = layout.split()
        col = split.column(align=True)
        col.operator("wm.add_death", text="Death", icon='MESH_CUBE')
        col.operator("wm.add_radiation", text="Radiation", icon='MESH_CUBE')
        col.operator("wm.add_win", text="Win", icon='MESH_CUBE')
