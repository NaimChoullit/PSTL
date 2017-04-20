type texture_t;;

external createTexture : string -> texture_t = "createTexture";;
external bindTexture : texture_t -> int -> unit = "bindTexture";;