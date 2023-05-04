# GraphicsTutorial
This is a personal project reference to https://www.youtube.com/@makinggameswithben/featured</br>

## 2D Game Engine</br>
### Classes
- IOManager : Read File
- GLSLProgram : Compile and Link Shaders
  - Use IOManager to read vertexShader and fragmentShader
  - Add Attribute to Program with addAttribute()
  - Get Uniform from program with getUniformLocation()
- ImageLoader : Load PNG with IO -> Decode & Convert to GLTexture
- GLTexture : Set Texture size and ID
- TextureCache : Contain Texture Cache
  - Get New Texture from ImageLoader with texturePath
  - Get Used Texture from textureMap (id - Texture) 
- ResourceManager : Wrapping Classes and Functions related to Resources
  - Get Texture using TextureCache
- Sprite : Set Entity and Texture</br>
    <img src="https://user-images.githubusercontent.com/76864202/232026064-4a9cb9c0-a781-4560-93e3-d891ba190c47.png" width="400" height="100">
- SpriteBatch : Draw groups of sprites onto the screen
  - begin() -> Objects call draw for batch -> end() -> renderBatches()
- Camera2D : Play as eyes of player
  - Update with Homogeneous Camera Matrix
  - Convert Screen Coordinate to World Coordinate
  - Optimization with Camera Culling (Ignore out of view)
- InputManager : Deal with User Input
  - Update Mouse Coordinates based on World Space according to Camera
  - Set and Update Key State (Pressed, Released, ...) with unordered_map
- Window : Create Program Window
  - Open an SDL Window : SDL_CreateWindow
  - Contain screenWidth, screenHeight
- Timing : Set Max FPS and Calculate FPS

- ParticleBatch2D : Draw groups of particles onto the screen
- ParticleEngine2D : Manage groups of ParticleBatch
- ScreenList : Manage list of screen
  - Screen Scrolling with moveNext, movePrev
  - Get and Set Current Screen Idx

### Interfaces
- IMainGame : Interface of MainGame
- IGameScreen : Interface of GameScreen

#
## Zombie Game</br>
- Map Setting with Level.txt by using File I/O</br>
- Class Inheritance : Agent <- Human <- Player</br>
- Moving(SDL_A, S ,D, W), Change Weapon(SDL_1, 2, 3), Shoot toward Mouse Coordinate</br>
- Collision Detection with AABB and Circle Radius</br>
- Display Text UI by using SDL_ttf</br>
- Particle Effect with Random Direction Vector and Life Time

https://user-images.githubusercontent.com/76864202/231960375-ca9ae5c4-047d-4ced-b9cc-98f05126f8c4.mp4

#
## Ball Graphics</br>
- Use Simple Physics : Billiards Collision, Position-Velocity-Acceleration Relation</br>
- Multiple Renderer : Render Mode with Transition, Momentum, Velocity</br>
- Optimization with Spatial Partitioning</br>

https://user-images.githubusercontent.com/76864202/231959477-14b39608-62eb-4864-a790-1f4133ca3a98.mp4

#
## Ninja Platformer</br>
- Box2D Physics 101</br>
- Box Collider && Capsule Collider</br>
- 2D Animation w/ sprite and uvRect</br>
- Basic 2D Lighting</br>

//play video comes here</br>

#
TODO : </br>
1. Mouse Coordinate Error : x coord is tracking wrong!!
