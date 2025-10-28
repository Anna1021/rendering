import bpy
import mathutils
import math

def export_scene(filepath):
    with open(filepath, "w") as f:

        # === Camera ===
        cam = bpy.context.scene.camera
        mat = cam.matrix_world

        pos = mat.translation
        pos_r = (pos.x, pos.z, -pos.y)

        forward = -(mat.to_3x3() @ mathutils.Vector((0, 0, 1)))
        up = (mat.to_3x3() @ mathutils.Vector((0, 1, 0)))

        forward_r = (forward.x, forward.z, -forward.y)
        up_r = (up.x, up.z, -up.y)

        resx = bpy.context.scene.render.resolution_x
        resy = bpy.context.scene.render.resolution_y
        fov = cam.data.angle * 180.0 / math.pi

        f.write("camera\n")
        f.write(f"{pos_r[0]:.4f} {pos_r[1]:.4f} {pos_r[2]:.4f}\n")
        f.write(f"{forward_r[0]:.4f} {forward_r[1]:.4f} {forward_r[2]:.4f}\n")
        f.write(f"{up_r[0]:.4f} {up_r[1]:.4f} {up_r[2]:.4f}\n")
        f.write(f"{fov:.4f}\n")
        f.write(f"{resx} {resy}\n\n")

        # === Lights ===
        for obj in bpy.data.objects:
            if obj.type == 'LIGHT':
                loc = obj.location
                loc_r = (loc.x, loc.z, -loc.y)
                intensity = obj.data.energy
                f.write(f"light {loc_r[0]:.4f} {loc_r[1]:.4f} {loc_r[2]:.4f} {intensity:.4f}\n")

        f.write("\n")

        # === Mesh Objects ===
        for obj in bpy.data.objects:
            if obj.type != "MESH":
                continue

            name = obj.data.name.lower()

            # Sphere
            if "sphere" in name:
                loc = obj.location
                loc_r = (loc.x, loc.z, -loc.y)
                scale = obj.scale.x
                f.write(f"sphere {loc_r[0]:.4f} {loc_r[1]:.4f} {loc_r[2]:.4f} {scale:.4f}\n")

            # Cube
            elif "cube" in name:
                loc = obj.location
                loc_r = (loc.x, loc.z, -loc.y)
                rx, ry, rz = obj.rotation_euler
                f.write(f"cube {loc_r[0]:.4f} {loc_r[1]:.4f} {loc_r[2]:.4f} {obj.scale.x:.4f} {rx:.4f} {ry:.4f} {rz:.4f}\n")

            # Plane (correct vertex order)
            elif "plane" in name:
                mesh = obj.data

                # ✅ 正确使用 polygon 获取顶点顺序（而不是 verts[0:4]）
                face = next(f for f in mesh.polygons if len(f.vertices) == 4)
                idx = face.vertices[:]  # 四个角点正确顺序

                verts = [obj.matrix_world @ mesh.vertices[i].co for i in idx]

                # 坐标转换
                verts_r = [(v.x, v.z, -v.y) for v in verts]

                f.write("plane ")
                for (x, y, z) in verts_r:
                    f.write(f"{x:.4f} {y:.4f} {z:.4f} ")
                f.write("\n")

    print(f"✅ Scene exported → {filepath}")

# === 导出 ===
export_scene("/Users/xinxinzi/Documents/postgraduate/rendering/cw1/ASCII/scene.txt")
