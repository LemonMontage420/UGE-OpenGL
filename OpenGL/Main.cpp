//TODO:
// 
// Possibly Replace All Delete Functions With Destructors In Their Class (Will Have To Actually Research C++ A Bit More And Git Gud)
// 
// Fix Bug Where Every Mesh Would Just Use Whatever's Tied To Texture Slot 0 (Fixed, Was A Misconception On My Part, We First Activate The
// Texture Slot We Wanna Modify, And Then Either Bind The Current Mesh's Texture To It, Or Unbind The Texture Data From It,
// So That Multiple Objects Can Use The Same Texture Slots (Activate Object -> Activate Texture Slot -> Bind Object's Texture Data
// To Said Texture Slot -> Set The Texture Slot Uniform In The Object's Shader So We Can Link Each sampler2D Uniform To Its
// Respective Texture Slot -> Draw The Object -> Unbind The Texture From The Texture Slots So That The Next Object Can Use
// The Texture Slots After Us))
// 
// Fix The Issue Where Not Everything Is Updating Properly When The Window Resizes (Mainly The Origin Point For The Mouse Position)
// (Fixed, Should Try And Also Make It So There's A Toggleable Option To Adjust The FOV Aswell When The Window Resizes To
// Preserve The Things On The Screen And Prevent Players From Being Able To Look Out More To The Sides By Changing Their Aspect Ratio)
// 
// Fix Issue Where The Application Crashes When The Window Gets Minimized
// 
// Have Everything Be Tied To Delta Time (Fixed, Camera Movement Controls Are Tied To It Now, Still Need The Model Matrix Stuff To Use Delta Time Though)
// 
// Make Controls To Set The Resolution And Aspect Ratio Of The Window Aswell As Toggle Fullscreen
// 
// Refactor The Input To Be Its Own Thing And Set Callbacks For When We Simply Tap A Key Instead Of When We Hold It Down
// 
// Get Some Basic Shape Generation Working (Triangle(3D And 2D), Quad (3D And 2D), Circle (3D And 2D), 
// Cube, Pyramid, Icosphere (With Customizable LOD), UV Sphere (With Customizable LOD)) (Done, Just Need To Validate The Cylinder And Torus)
// 
// Make A Material Class That Handles The Creation And Linking Of Shader Data To A Pre-Defined Shader Program.
// Different Materials Can Have The Same Shader, Different Objects Can Use The Same Material (Also Try And Make The
// Material Class Aware Of What Data (Uniforms) Need To Be Filled Out In The Shader, And Then Have The Class
// Only Request That Data When Being Constructed)
// 
// Have It So The Same Mesh Can Have Multiple Materials (Mostly In Place Thanks To Assimp, Assimp Automatically Splits Meshes Up Into "Sub Meshes"
// In Which Each Submesh Uses One Material, This Is Apparent When We Tick The "Material" Box When Exporting Our Model From Blender,
// As There's Now Multiple Meshes If The Model Had Multiple Materials Assigned To It, Also Make Sure To Test What Happens When There's
// A Material Assigned To The Model That Isn't Being Used (Although I Should Try To Avoid This), Aswell As See If I Can Optimize/Fix Any
// Issues That Arise If This Is The Case (Like Meshes Being Instantiated With Zero Data Linked To Them), I Can Also Get The Submesh's
// Material Index, Aswell As That Material's Name When Importing The Mesh
// 
// Get Basic Lighting Working With Proper Light Handling And A Material Class And Whatnot Figured Out Before Switching To PBR
// 
// Have The Model Matrix Able To Modify Both The Local TRS And Global TRS Like In Unity
// 
// Create A "Game Object" Class, Which Can Have Multiple Components Inside Of It, 
// Of Which, A Mesh Can Be One Of Them, Object Transform Is Another, Object Material Is Another, etc.
// 
// Get Model Loading Working (Done, Experiment With Importing Different Model Formats, With Different Properties And Different Transforms,
// And Make An Excel Sheet Documenting What Each Format Does)
// 
// Get Transparency Working (Preferably With OIT)
// 
// Get More Camera Options Working, Like Culling Masks
// 
// Try And Completely Remove The Spatial Jittering Issue At Far Away Distances From The World Origin Caused By Floating Point Precision
// Limitations (Could Try And Use Doubles In Shaders, Aswell As Try And Use 32 Bit Depth Buffers)
// 
// Get Frambuffers In And Working
// 
// Have Basic Shaders Built In Like In Unity (Default Shader, Error Shader (Just Renders Magenta), Wireframe Shader)
// 
// Get A Basic Shadow Implementation Going (Directional, Point, And Spotlight Shadows)
// 
// Get A Better Debug System Setup As The Current One Is Really Lacking
// 
// Get The Skybox Working (Have The Options For Procedural Sky (Just Yoink The Code From The VP Tutorial Project), Cubemap, And Panorama)
// 
// Get The PBR Implementation Going
// 
// Get Anti-Aliasing (MSAA, FXAA, TAA), HDR, Tonemapping And Color Correction, And Bloom Up And Running (Make Sure The Bloom Uses A Box Blur Technique
// Aswell As Have It Be Written In A Way In Which It'll Be Easy To Convert It To A Compute Shader Later)
// 
// Add SSAO And Contact Shadows
// 
// Improve The Shadow Rendering To Use Cascaded Shadows And Nvidia PCSS
// 
// Add More Post-Processing Effects Like Auto Exposure, SSGI, SSRflections, SSRefractions,
// Chromatic Abberation, Vignette, Depth Of Field, And Film Grain
// 
// Replace The Bloom Implementation For A Convolution Bloom Implementation (Should Have Built-In Lens Flares I Believe)
// 
// Replace The Occlusion Implementation (SSAO, Contact Shadows/SSShadows, Shadow Maps) With Raytraced Shadows With Proper Umbra And Penumbra Support
// 
// Potentially Replace The Reflection Implementation (SSReflection, SSRefractions, Reflection Probes) With Raytraced Reflections
// 
// Get Per-Object Motion Blur In
// 
// 
// 
// Get To A Point Where I Only Have To Think About The Implementation Details Of What I'm Working On Instead Of Worrying About
// How I'd Go About Doing It Within The Language (Get Better At C++)
// 
// 
// 
// Get To A Point Where I Have Multiple Imported Models Rendering In Realtime With PBR
// (With Both Imported Textures And Procedural Textures That Get Saved Into A File
// And Read Back And Full On Procedural Shaders) With Some Interactivity (Directly Through OpenGL
// Through Keypresses And Whatnot Aswell As Through ImGUI) Along With Post-Processing Effects
// (Anti Aliasing, Auto Exposure, Fog, Vignette, Depth Of Field, Bloom, Ambient Occlusion, 
// Tonemapping, Screen Space Shadows) Aswell As Proper Transparency, HDR, And Shadows (With Multiple Lights
// Of Different Types) With It All Being Organized And Well Commented And Ready For General Purpose Use
// In A Non GUI Based 3D Game Engine

#include"Mesh.h"
#include"Model.h"
#include"Material.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void PollWindowEvents(GLFWwindow* window);

//int montiorCount;
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

//Construct The Camera Given A Screen Width, Height, FOV, Near Clip Plane, Far Clip Plane, And Initial Camera Position
//(Camera Needs To Be A Global Variable As The Framebuffer Resize Callback Can't Take The Camera As A Local Variable,
//And Obviously The Camera's Settings Need To Be Updated Whenever The Window Size Changes)
Camera camera(SCR_WIDTH, SCR_HEIGHT, 45.0f, 0.1f, 100.0f, glm::vec3(-0.5f, 0.5f, 4.0f));

double currentTime;
double lastTime;
double deltaTime;
float currentFPS;
float averageFPSOverInterval;
float counter;
int counterNum;

unsigned int textureSlot0 = 0;
unsigned int textureSlot1 = 1;
unsigned int textureSlot2 = 2;
glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

glm::vec3 modelPosition = glm::vec3(-0.5f, 0.0f, 0.0f);
glm::vec3 modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 lightRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightScale = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f);

int main() 
{
	//Initialize GLFW (Windowing And Inputs Library)
	glfwInit();

	//Setup GLFW By Telling It The OpenGL Specification We're Using (Core Version 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Get The Number Of Monitors Connected To This Device
	//GLFWmonitor** monitors = glfwGetMonitors(&montiorCount);

	//Create A Window With A Width, Height, And Title
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);

	//Error Checking Just Incase The Window Failed To Initialize
	if (window == NULL) 
	{
		std::cout << "Failed To Create GLFW Window" << std::endl;
		glfwTerminate();
	}

	//Make The Window We Just Configured The Current Context
	glfwMakeContextCurrent(window);

	//Enable Vsync With A Swap Interval Of 1 (Set To 0 To Disable Vsync)
	glfwSwapInterval(0);

	//Have The Window Resize Properly When The Window Gets Resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load The Correct OpenGL Functions Based Off Our Configuration Using GLAD
	gladLoadGL();

	//Error Checking Just Incase GLAD Failed To Initialize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed To Initialize GLAD" << std::endl;
	}

	 
	//Import Our Model And Construct It Using The Imported Data
	Model model("Countach.gltf");
	//Construct The Shader We'll Be Using To Render Our Mesh(s)
	Shader modelShader("Model.vert", "Model.frag");
	//Construct The Material We'll Be Using To Render Our Mesh(s)
	Material modelMaterial(modelShader);

	//Create An Array Of Model Textures (Temporary Until I Get Textures Importing)
	Texture modelTextures[] =
	{
		Texture("diffuse3.jpg", textureSlot0, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGB, GL_RGB, true),
		Texture("diffuse2.jpg", textureSlot1, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGB, GL_RGB, true),
		Texture("diffuse1.png", textureSlot2, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGBA, GL_RGBA, true)
	};
	//Create The CPP Vector From The Texture Array Created Above
	std::vector<Texture> modelTex(modelTextures, modelTextures + sizeof(modelTextures) / sizeof(Texture));

	//Initialize The Model Transforms For The Model Matrix
	Transform modelTransform = { modelPosition, modelRotation, modelScale };

	Uniform modelUniArray[] =
	{
		Uniform{"model0"},
		Uniform{"model1"}
	};
	std::vector<Uniform> modelUniforms(modelUniArray, modelUniArray + sizeof(modelUniArray) / sizeof(Uniform));
	modelShader.Activate();
	for (int i = 0; i < modelTex.size(); i++)
	{
		std::string currentTex = std::to_string(i);
		modelShader.setInt("texture" + currentTex, i);
	}
	modelShader.setVec3("lightPosition", lightPosition);
	modelShader.setVec3("lightColor", lightColor);
	modelShader.Deactivate();


	Primitive prim = Primitive::Cylinder;
	//Import Our Model And Construct It Using The Imported Data
	Mesh light(prim, 10);
	//Construct The Shader We'll Be Using To Render Our Mesh(s)
	Shader lightShader("Light.vert", "Light.frag");
	//Construct The Material We'll Be Using To Render Our Mesh(s)
	Material lightMaterial(lightShader);

	Texture lightTextures[] =
	{
		Texture("diffuse3.jpg", textureSlot0, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGB, GL_RGB, true),
		Texture("diffuse2.jpg", textureSlot1, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGB, GL_RGB, true),
		Texture("diffuse4.png", textureSlot2, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, borderColor, GL_RGBA, GL_RGBA, true)
	};
	//Create The CPP Vector From The Texture Array Created Above
	std::vector<Texture> lightTex(lightTextures, lightTextures + sizeof(lightTextures) / sizeof(Texture));

	Transform lightTransform = { lightPosition, lightRotation, lightScale };

	Uniform lightUniArray[] =
	{
		Uniform{"light0"},
		Uniform{"light1"}
	};
	std::vector<Uniform> lightUniforms(lightUniArray, lightUniArray + sizeof(lightUniArray) / sizeof(Uniform));
	//Temp Way To Send Shader-Specific Uniforms Through
	lightShader.Activate();
	for (int i = 0; i < lightTex.size(); i++)
	{
		std::string currentTex = std::to_string(i);
		lightShader.setInt("texture" + currentTex, i);
	}
	lightShader.setVec3("lightColor", lightColor);
	lightShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 1.0f));
	lightShader.Deactivate();


	Primitive primDebug = Primitive::Cube;
	std::vector<Mesh> debugMeshes;
	for (unsigned int i = 0; i < light.vertices.size(); i++)
	{
		Mesh debug(primDebug, 0);
		debugMeshes.push_back(debug);
	}
	Shader debugShader("Debug.vert", "Debug.frag");
	std::vector<Transform> debugTransforms;
	for (unsigned int i = 0; i < light.vertices.size(); i++)
	{
		Transform debug = { (light.vertices[i].position + lightPosition) + (light.vertices[i].normals * 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f) };
		debugTransforms.push_back(debug);
	}
	debugShader.Activate();
	debugShader.setVec3("debugColor", glm::vec3(1.0f, 1.0f, 1.0f));
	debugShader.Deactivate();

	//Enable Depth Testing/Depth Buffer
	glEnable(GL_DEPTH_TEST);

	//Cull The Back Faces Of The Triangles
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); //Set The Winding Order To Counter-Clockwise (The Industry Standard)
	glCullFace(GL_BACK);

	//Draw Polygons As Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //NOTE: using GL_FRONT or GL_BACK won't work as they're deprecated
	

	float updateRate = 5.0f; //Number Of Times We Update The FPS Counter Per Second
	//Main Loop
	while (!glfwWindowShouldClose(window))
	{
		//Get Delta Time
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		currentFPS = 1.0f / (float)deltaTime;
		averageFPSOverInterval += currentFPS;
		counter += (float)deltaTime;
		counterNum++;

		if (counter >= 1.0f / updateRate)
		{
			averageFPSOverInterval /= counterNum;
			//Set The Window Title Every Frame To The Current Frame Rate
			glfwSetWindowTitle(window, std::to_string(averageFPSOverInterval).c_str());
			counter -= (1.0f / updateRate);
			counterNum = 0;
			averageFPSOverInterval = 0.0f;
		}

		//Record And Process Inputs
		glfwPollEvents();
		PollWindowEvents(window);
		camera.PollCameraEvents(window, (float)deltaTime);

		//Clear The Back Buffer With Our Set Clear Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update The Camera Matrix After We've Recorded The Camera Events Of This Frame, The Matrix Then Gets Set In The Mesh's Shader When We Issue A Draw Call
		camera.UpdateMatrix();



		//Update The Model Matrices Components (Temporary Location, Should Be Moved Into A "Transform" Class Or Something)
		modelRotation += glm::vec3(90.0f * deltaTime, 0.0f * deltaTime, 0.0f);
		//Set The Model Matrix After We've Recorded The Input Events Of This Frame
		modelTransform = { modelPosition, modelRotation, modelScale };
		for (unsigned int i = 0; i < modelTex.size(); i++)
		{
			modelTex[i].Bind();
		}
		//Temp Way To Update Shader-Specific Data
		modelShader.Activate();
		modelShader.setVec3("cameraPosition", camera.camPosition);
		modelShader.Deactivate();
		//Draw Our Model(s) (Might Seem Weird Right Now That The Mesh Class Is Concerned With Camera Stuff And Is Responsible For Setting The
		//Camera Matrix In The Shader, But Later On, Meshes Can Be Configured To Only Be Drawn By Certain Cameras Via Culling Masks)
		for (unsigned int i = 0; i < model.meshes.size(); i++)
		{
			model.meshes[i].Draw(modelShader, camera, modelTransform);
		}
		for (unsigned int i = 0; i < modelTex.size(); i++)
		{
			modelTex[i].Unbind();
		}

		//lightRotation += glm::vec3(0.0f, 90.0f * deltaTime, 0.0f);
		lightTransform = { lightPosition, lightRotation, lightScale };
		for (unsigned int i = 0; i < lightTex.size(); i++)
		{
			lightTex[i].Bind();
		}
		light.Draw(lightShader, camera, lightTransform);
		for (unsigned int i = 0; i < lightTex.size(); i++)
		{
			lightTex[i].Unbind();
		}

		for (unsigned int i = 0; i < debugMeshes.size(); i++)
		{
			debugMeshes[i].Draw(debugShader, camera, debugTransforms[i]);
		}

		//Swap The Front Buffer (What Gets Displayed On The Screen) With The Back Buffer
		glfwSwapBuffers(window);
	}



	//De-Initialize
	for (unsigned int i = 0; i < modelTex.size(); i++)
	{
		modelTex[i].Delete();
	}
	for (unsigned int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i].Delete();
	}
	modelShader.Delete();

	for (unsigned int i = 0; i < lightTex.size(); i++)
	{
		lightTex[i].Delete();
	}
	light.Delete();
	lightShader.Delete();

	for (unsigned int i = 0; i < debugMeshes.size(); i++)
	{
		debugMeshes[i].Delete();
	}
	debugShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//Query GLFW Whether Relevant Keys Are Pressed/Released This Frame And React Accordingly
void PollWindowEvents(GLFWwindow* window)
{
	//Set The Boolean To Close The Window To True If We Press The Escape Key
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;

	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
	camera.UpdateViewportSize(SCR_WIDTH, SCR_HEIGHT);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}