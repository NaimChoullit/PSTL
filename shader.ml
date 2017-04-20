type shader_t;;
type transform_t;;
type camera_t;;

external createShader : string -> shader_t = "createShad";;
external bindShader : shader_t -> unit = "bindShad";;
external createTransform : unit -> transform_t = "createTransform"
external updateShader : shader_t -> transform_t -> camera_t -> unit = "updateShader"

external setPosX : transform_t -> float -> unit = "setPosX"
external setPosY : transform_t -> float -> unit = "setPosY"
external setPosZ : transform_t -> float -> unit = "setPosZ"
external setRotX : transform_t -> float -> unit = "setRotX"
external setRotY : transform_t -> float -> unit = "setRotY"
external setRotZ : transform_t -> float -> unit = "setRotZ"
external setScaleX : transform_t -> float -> unit = "setScaleX"
external setScaleY : transform_t -> float -> unit = "setScaleY"
external setScaleZ : transform_t -> float -> unit = "setScaleZ"

external createCamera : float array -> float -> float -> float -> float -> camera_t = "createCamera"
