import bpy
import mathutils

def export_scene(filepath):
    with open(filepath, "w") as f:
        # === Camera ===
        cam = bpy.context.scene.camera
        mat = cam.matrix_world

        pos = mat.translation
        forward = -(mat.to_3x3() @ mathutils.Vector((0, 0, 1)))
        up = (mat.to_3x3() @ mathutils.Vector((0, 1, 0)))

        resx = bpy.context.scene.render.resolution_x
        resy = bpy.context.scene.render.resolution_y
        fov = cam.data.angle * 180.0 / 3.1415926  # 弧度→角度

        f.write("camera\n")
        f.write(f"{pos.x:.4f} {pos.y:.4f} {pos.z:.4f}\n")
        f.write(f"{forward.x:.4f} {forward.y:.4f} {forward.z:.4f}\n")
        f.write(f"{up.x:.4f} {up.y:.4f} {up.z:.4f}\n")
        f.write(f"{fov:.4f}\n")
        f.write(f"{resx} {resy}\n\n")

        # === Lights ===
        for obj in bpy.data.objects:
            if obj.type == 'LIGHT':
                loc = obj.location
                intensity = obj.data.energy
                f.write(f"light {loc.x:.4f} {loc.y:.4f} {loc.z:.4f} {intensity:.4f}\n")

        f.write("\n")

        # === Objects ===
        for obj in bpy.data.objects:
            if obj.type != "MESH":
                continue

            name = obj.data.name.lower()

            # Sphere
            if "sphere" in name:
                f.write(f"sphere {obj.location.x:.4f} {obj.location.y:.4f} {obj.location.z:.4f} {obj.scale.x:.4f}\n")

            # Cube
            elif "cube" in name:
                rx, ry, rz = obj.rotation_euler
                f.write(f"cube {obj.location.x:.4f} {obj.location.y:.4f} {obj.location.z:.4f} {obj.scale.x:.4f} {rx:.4f} {ry:.4f} {rz:.4f}\n")

            # Plane
            elif "plane" in name:
                mesh = obj.data
                verts = [obj.matrix_world @ v.co for v in mesh.vertices[:4]]
                coords = " ".join([f"{v.x:.4f} {v.y:.4f} {v.z:.4f}" for v in verts])
                f.write(f"plane {coords}\n")

    print(f"✅ Scene exported to {filepath}")


export_scene("/Users/xinxinzi/Documents/postgraduate/rendering/cw1/ASCII/scene.txt")

