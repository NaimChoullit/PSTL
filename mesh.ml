type mesh_t

external createMesh : float array array array -> mesh_t = "createMesh";;
external createMeshWithFile : string -> mesh_t = "createMeshWithFile"
external drawMesh : mesh_t -> unit = "drawMesh";;