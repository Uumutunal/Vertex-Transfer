import bpy
import bmesh
from mathutils import kdtree, Vector
from bpy.types import (Panel, Operator)
import ctypes as ct
import numpy as np
import time


bl_info = {
    "name": "Multiresolution - Delete Lower",
    "author": "Your Name Here",
    "version": (1, 0),
    "blender": (3, 6, 2),
    "location": "Sidebar (N) -> Tools panel",
    "description": "Deletes lower subdivision levels",
    "warning": "",
    "doc_url": "",
    "category": "Add Mesh",
}


#Transfer index order from source object to target object
#This is needed in order to use reshape in multires modifier
def index_transfer(src_obj, trgt_obj):
    sourceObj = src_obj
    targetObj = trgt_obj
    
    start_time = time.time()
    kdtree_c = ct.WinDLL(r"C:\Program Files (x86)\Blender Dosyaları\Scripting\MultiresDelLower\KDTree\x64\Release\KDTree.dll")
    # Define the function signatures
    kdtree_c.insert.argtypes = (ct.c_float, ct.c_float, ct.c_float, ct.c_int)
    kdtree_c.insert.restype = None

    kdtree_c.findAt.argtypes = (ct.c_int,)
    kdtree_c.findAt.restype = ct.c_int

    kdtree_c.find.argtypes = (ct.c_float, ct.c_float, ct.c_float)
    kdtree_c.find.restype = ct.c_int

    kdtree_c.dist.argtypes = (ct.c_float, ct.c_float, ct.c_float)
    kdtree_c.dist.restype = ct.c_int
    kdtree_c.init()
    kdtree_c.insert(999, 999, 999, -1)
    
    kdtree_c.initDict.argtypes = None
    kdtree_c.initDict.restype = None 
    
    kdtree_c.addToDict.argtypes = (ct.c_float, ct.c_float, ct.c_int)
    kdtree_c.addToDict.restype = None
    
    kdtree_c.findInDict.argtypes = (ct.c_float, ct.c_float)
    kdtree_c.findInDict.restype = ct.c_int 
    
    #kdtree_c.initDict()
    
    #kdtree_c.find.restype = ct.c_int
    #kdtree_c.find.argtypes = [ct.c_float]
    #kdtree_c.insert(ct.c_float(10), ct.c_float(10), ct.c_float(10), ct.c_int(10))
    #kdtree_c.find(ct.c_float(10), ct.c_float(10), ct.c_float(10), ct.c_int(10))
    
    bm_src = bmesh.new()
    bm_src.from_mesh(sourceObj.data)
    bm_src.faces.ensure_lookup_table()
    
    src_vert_indexed_dict = {}
    src_obj_kd_verts = kdtree.KDTree(len(bm_src.verts))
    
    #Loop through vertecies in the source object and find uv coordinates
    for vert in bm_src.verts:
        uv_layer = bm_src.loops.layers.uv.active
        # Access UV coordinates of the vertex
        uv_coords = vert.link_loops[0][uv_layer].uv

        uv_co = (uv_coords.x,uv_coords.y,0)
        src_obj_kd_verts.insert(uv_co, vert.index)
        
        
        i_x = ct.c_float(uv_co[0])
        i_y = ct.c_float(uv_co[1])
        i_z = ct.c_float(uv_co[2])
        
        ind = ct.c_int(vert.index)
        
        kdtree_c.addToDict(i_x,i_y,ind)
        
        
        '''
        print("######")
        print(i_x, i_y, i_z, ind)
        print("vert.index: " ,vert.index)
        print("ind: " , ind)
        print("uv_co[0]: ", uv_co[0])
        print("find: ", kdtree_c.find(i_x, i_y, i_z))
        print(kdtree_c.findAt(vert.index))
        print("dist: ", kdtree_c.dist(i_x, i_y, i_z))
        src_obj_kd_verts.balance()
        co, index, dist = src_obj_kd_verts.find(uv_co)
        print("kd: ", index)
        print("dict: ", kdtree_c.findInDict(i_x,i_y))
        '''

        

    
    #
    #dll
    
    
    #
    #kdtree_c.insert(f,6)
    print("asd")
    #print(kdtree_c.findAt(10))
    #print(kdtree_c.find(ct.c_float(10)))
    #
    
    bm_trg = bmesh.new()
    bm_trg.from_mesh(trgt_obj.data)
    ##src_obj_kd_verts.balance()
    
    #Loop through the target object and find matching vertecies
    for vert in bm_trg.verts:
        uv_layer = bm_trg.loops.layers.uv.active
        # Access UV coordinates of the vertex
        uv_coords = vert.link_loops[0][uv_layer].uv

        uv_co = (uv_coords.x,uv_coords.y,0)
        ##co, index, dist = src_obj_kd_verts.find(uv_co)
        
        
        i_x = ct.c_float(uv_co[0])
        i_y = ct.c_float(uv_co[1])
        i_z = ct.c_float(uv_co[2])
        
        #print("############")
        #vert.index = kdtree_c.dist(i_x, i_y, i_z)
        #a = kdtree_c.dist(i_x, i_y, i_z)
        #print("dict: ", kdtree_c.findInDict(i_x,i_y))
        
        vert.index = kdtree_c.findInDict(i_x,i_y)
        
        #If the uv coordinates matches, change the vertex index
        ##if dist < 0.1:
            ##vert.index = index
            #print("kdtree: ", index)
            #vert.index = kdtree_c.dist(i_x, i_y, i_z)
            
            #print("find: ", kdtree_c.dist(i_x, i_y, i_z))
            #print("kd: ", index)


    bm_trg.verts.sort()
    bm_trg.edges.sort()
    bm_trg.faces.sort()
    bm_trg.to_mesh(targetObj.data)
    bm_trg.free()
    bm_src.free()
    
    end_time = time.time()
    execution_time = end_time - start_time
    print("Execution Time:", execution_time, "seconds")
    print("FINISHED")
    return {"FINISHED"}


#Turn off the smoothing before applying multires modifier in order to keep the uv's exact
def apply_multires():
    bpy.context.object.modifiers["Multires"].uv_smooth = 'NONE'
    bpy.context.object.modifiers["Multires"].boundary_smooth = 'PRESERVE_CORNERS'
    bpy.ops.object.modifier_apply(modifier="Multires", report=True)



def del_lower():
    objs = []
    active_obj = bpy.context.active_object
    
    bpy.context.object.modifiers["Multires"].uv_smooth = 'NONE'
    bpy.context.object.modifiers["Multires"].boundary_smooth = 'PRESERVE_CORNERS'
    
    #The subdivision level to be deleted
    del_lower = bpy.context.object.modifiers["Multires"].levels
    
    #Highest subdivision level
    del_lower_duplicated = bpy.context.object.modifiers["Multires"].render_levels
        

    #dup1
    bpy.ops.object.duplicate(linked=0,mode='TRANSLATION') 
    duplicate_obj = bpy.context.active_object
    duplicate_obj.name = "dup1"
    objs.append(duplicate_obj)
    #
    
    #dup2
    bpy.ops.object.duplicate(linked=0,mode='TRANSLATION') 
    duplicate2_obj = bpy.context.active_object
    duplicate2_obj.name = "dup2"
    apply_multires()
    objs.append(duplicate2_obj)
    #

    
    bpy.context.view_layer.objects.active = duplicate_obj
    bpy.context.object.modifiers["Multires"].levels = del_lower
    apply_multires()
    

    bpy.ops.object.modifier_add(type='MULTIRES')
    bpy.context.object.modifiers["Multires"].uv_smooth = 'NONE'
    bpy.context.object.modifiers["Multires"].boundary_smooth = 'PRESERVE_CORNERS'
    for i in range(del_lower, del_lower_duplicated):
        bpy.ops.object.multires_subdivide(modifier="Multires", mode='CATMULL_CLARK')
    apply_multires()
    

    bpy.context.view_layer.objects.active = active_obj
    
    apply_multires()
    
    bpy.ops.object.modifier_add(type='MULTIRES')
    bpy.context.object.modifiers["Multires"].uv_smooth = 'NONE'
    bpy.context.object.modifiers["Multires"].boundary_smooth = 'PRESERVE_CORNERS'
    for i in range(del_lower, del_lower_duplicated):
        bpy.ops.object.multires_subdivide(modifier="Multires", mode='CATMULL_CLARK')
    
    
    #execute(bpy.context, duplicate3_obj, duplicate2_obj)
    index_transfer(duplicate_obj, duplicate2_obj)

    bpy.ops.object.select_all(action='DESELECT')

    object_names_to_select = [duplicate2_obj.name, active_obj.name]

    for obj in bpy.data.objects:
        if obj.name in object_names_to_select:
            obj.select_set(True)
    bpy.context.view_layer.objects.active = active_obj
    

    bpy.ops.object.multires_reshape(modifier="Multires")
    
    for obj in objs:
        bpy.data.objects.remove(bpy.data.objects[obj.name], do_unlink=True)

    bpy.context.object.modifiers["Multires"].uv_smooth = 'PRESERVE_BOUNDARIES'
    bpy.context.object.modifiers["Multires"].boundary_smooth = 'ALL'




class DeleteLower(Operator):
    """Delete lower subdivision levels"""
    bl_idname = "object.delete_lower"
    bl_label = "Delete Lower"

    def execute(self, context):
        for modifier in bpy.context.active_object.modifiers:
            if modifier.type == "MULTIRES":
                del_lower()
        
        return {'FINISHED'}


class TransferVertexIndex(Operator):
    """Copy vertex order from active object to selected object"""
    bl_idname = "object.copy_index"
    bl_label = "Transfer Vertex Order"
    
    def execute(self, context):
        
        print("")
        return {'FINISHED'}    


class OBJECT_PT_CustomPanel(Panel):
    bl_idname = "object.custom_panel"
    bl_label = "Multiresolution Edit"
    bl_space_type = "VIEW_3D"   
    bl_region_type = "UI"
    bl_category = "Tools"
    bl_context = "objectmode"   


    @classmethod
    def poll(self,context):
        return context.object is not None

    def draw(self, context):
        layout = self.layout
        obj = context.object

        col = layout.column(align=True)
        row = col.row(align=True)
        
        col.operator(TransferVertexIndex.bl_idname)
        col.operator(DeleteLower.bl_idname)

        layout.separator()

classes = (
    OBJECT_PT_CustomPanel,
    DeleteLower,
    TransferVertexIndex
)



def register():
    for cls in classes:
        bpy.utils.register_class(cls)

def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)

if __name__ == "__main__":
    register()