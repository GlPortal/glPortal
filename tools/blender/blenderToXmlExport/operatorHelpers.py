import bpy
import os

def clearGlpProperties():
    object = bpy.context.active_object
    if object:
        object.glpTypes = "none"
        object.glpVolumeTypes = "none"
        object.glpTriggerTypes = "none"
        object.glpWallTypes = "none"

def getMaterial(texturePath, diffuse, dimensions = [1,1,1]):
    realpath = os.path.expanduser(texturePath)
    try:
        WallImage = bpy.data.images.load(realpath)
    except:
        raise NameError("Cannot load image %s" % realpath)
    
    WallTexture = bpy.data.textures.new(name = 'ColorTex', type = 'IMAGE')
    WallTexture.image = WallImage
    
    mat = bpy.data.materials.new('TexMat')
    mat.diffuse_color = diffuse
    
    # Add texture slot for color texture
    mtex = mat.texture_slots.add()
    mtex.texture = WallTexture
    mtex.texture_coords = 'GLOBAL'
    mtex.use_map_color_diffuse = True
    mtex.use_map_color_emission = True
    mtex.emission_color_factor = 0.5
    mtex.use_map_density = True
    mtex.mapping = 'CUBE'
    mtex.use_map_emit = True
    mtex.emit_factor = 0.3
#    mtex.offset = [0, -0.25, 0]
    
    return mat
