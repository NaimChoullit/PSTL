open Display;;
open Shader;;
open Mesh;;
open Texture;;
open Printf;;
open Printf;;


let vertices = [|
                  [| [|-0.5; -0.5; 0. |] ; [|0.5 ; 1. |] |];
                  [| [|0. ; 0.5;  0. |]; [|0.5; 1. |] |];
                  [| [|0.5; -0.5;  0. |]; [|1. ;  0. |] |]
                |]

let count = ref 0. ;;
let sinCounter = ref 0. ;;
let cosCounter = ref 0. ;;

let window = createWindow 800 600 "Ocaml 3D";;
let shader = createShader "./res/basicShader";;
let mesh = createMesh vertices;;
let meshWithFile = createMeshWithFile "./res/monkey3.obj";;
let texture = createTexture "./res/bricks.jpg";;
let transform = createTransform();;




let camera = createCamera [|0.0;0.0;-5.0|] 70.0 (800.0 /. 600.0) 0.01 1000.0;;
while isClosed(window) == false do
  clear window 0.0 0.15 0.3 1.0;
  
  sinCounter := sin !count;
  cosCounter := cos !count;

  setPosX transform !sinCounter;
  setPosZ transform !cosCounter;
  setRotX transform !cosCounter;
  setRotY transform (!sinCounter *. 3.);
  setRotZ transform (!cosCounter *. 2.);
  

  bindShader shader;
  bindTexture texture 0;
  updateShader shader transform camera;
  drawMesh meshWithFile;
  update window;
  count := !count +. 0.01 ;
  
  done;
  
   
  
