type window_ptr

external createWindow : int -> int -> string -> window_ptr= "createWindow"
external clear : window_ptr -> float -> float -> float-> float -> unit = "clear"

external isClosed : window_ptr -> bool = "isClosed"
external update : window_ptr -> unit = "update"
