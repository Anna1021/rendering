# Export.py  —— Blender → JSON (ASCII)
# 覆盖相机、点光、球、立方体、平面五类对象
import bpy, json, os
from mathutils import Vector, Matrix

# ====== 1) 改成你本机项目根目录的绝对路径 ======
project_dir = "/Users/xinxinzi/Documents/postgraduate/rendering/cw1/s2810619"
out_path = os.path.join(project_dir, "ASCII", "scene.json")
os.makedirs(os.path.dirname(out_path), exist_ok=True)

scene_data = {}

# ====== 小工具 ======
def world_verts(obj):
    """把 mesh 顶点转换到世界坐标"""
    mw = obj.matrix_world
    return [mw @ v.co for v in obj.data.vertices]

def is_cube(obj):
    """简单判断是否 cube：名字含 'cube' 或 8 顶点/6 面"""
    name_ok = obj.name.lower().startswith("cube")
    topol_ok = hasattr(obj.data, "polygons") and len(obj.data.polygons) == 6 and len(obj.data.vertices) == 8
    return name_ok or topol_ok

def is_sphere(obj):
    """简单判断是否 sphere：名字含 'sphere' 或顶点数很多且面数多"""
    return obj.name.lower().startswith(("sphere","uvsphere","ico","icosphere"))

def is_plane(obj):
    """判断是否平面：名字含 'plane' 或 4 顶点/1 面"""
    name_ok = obj.name.lower().startswith("plane")
    topol_ok = hasattr(obj.data, "polygons") and len(obj.data.polygons) == 1 and len(obj.data.vertices) == 4
    return name_ok or topol_ok

def plane_corners_world(obj):
    """导出平面的四个角（世界坐标，按顶点顺序输出）"""
    vs = world_verts(obj)
    # 如果不是严格 4 顶点，就取前4个（课程通常是4顶点的 plane）
    if len(vs) >= 4:
        return [list(v) for v in vs[:4]]
    return [list(v) for v in vs]

# ====== 2) Camera（取场景活动相机；若无，则取第一个）======
cam_obj = bpy.context.scene.camera
if not cam_obj:
    for o in bpy.data.objects:
        if o.type == 'CAMERA':
            cam_obj = o
            break

if cam_obj and cam_obj.type == 'CAMERA':
    cam = cam_obj.data
    # 相机 gaze：把相机局部 -Z 方向变换到世界坐标
    gaze_world = cam_obj.matrix_world.to_quaternion() @ Vector((0.0, 0.0, -1.0))
    scene_data["camera"] = {
        "location": list(cam_obj.location),
        "gaze": [gaze_world.x, gaze_world.y, gaze_world.z],
        "focal_length": float(cam.lens),                 # 毫米
        "sensor_width": float(cam.sensor_width),         # 毫米
        "sensor_height": float(cam.sensor_height),       # 毫米
        "resolution": [
            int(bpy.context.scene.render.resolution_x),
            int(bpy.context.scene.render.resolution_y)
        ]
    }

# ====== 3) Point Lights ======
lights = []
for o in bpy.data.objects:
    if o.type == 'LIGHT':
        L = o.data
        if L.type == 'POINT':
            col = L.color[:]  # (r,g,b) 0..1
            lights.append({
                "type": "point",
                "name": o.name,
                "location": list(o.location),
                # 记录能量和颜色，渲染端可把 radiant_intensity ~ energy * color 使用
                "color": [float(col[0]), float(col[1]), float(col[2])],
                "energy": float(L.energy)   # Blender 单位（W），你在 C++ 里按需换算
            })
if lights:
    scene_data["lights"] = lights

# ====== 4) Spheres ======
# 课程里没要求网格精确识别球，这里按名称判断（UVSphere/IcoSphere/Sphere）
spheres = []
for o in bpy.data.objects:
    if o.type == 'MESH' and is_sphere(o):
        # 半径：采用平均缩放后的半径（Blender 的 unit sphere 半径一般为 1）
        # 如果你的球是通过缩放得到，radius = 平均(scale) * 1
        s = o.scale
        radius = float((s.x + s.y + s.z) / 3.0)
        spheres.append({
            "name": o.name,
            "center": list(o.location),
            "radius": radius
        })
if spheres:
    scene_data["spheres"] = spheres

# ====== 5) Cubes ======
cubes = []
for o in bpy.data.objects:
    if o.type == 'MESH' and is_cube(o):
        cubes.append({
            "name": o.name,
            "location": list(o.location),
            "rotation_euler": [float(a) for a in o.rotation_euler],  # 弧度
            "scale": float(o.scale.x)  # 按要求：1D scale（假设均匀缩放）
        })
if cubes:
    scene_data["cubes"] = cubes

# ====== 6) Planes（四个角的世界坐标）======
planes = []
for o in bpy.data.objects:
    if o.type == 'MESH' and is_plane(o):
        corners = plane_corners_world(o)
        planes.append({
            "name": o.name,
            "corners": corners  # [[x,y,z], ... x4]
        })
if planes:
    scene_data["planes"] = planes

# ====== 7) 写文件 ======
with open(out_path, "w", encoding="utf-8") as f:
    json.dump(scene_data, f, indent=4)
print(f"[Export.py] Scene exported to: {out_path}")
