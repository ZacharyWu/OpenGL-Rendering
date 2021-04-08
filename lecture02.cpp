// Most OpenGL applications use standard C library (e.g printf), so
#include<stdio.h>
#include<stdlib.h>

// glew.h
#ifdef MACOSX
#include</opt/local/include/GL/glew.h>
#else
#include <GL/glew.h>
#endif

// glut.h
#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



//Golbal variables
char title[] = "3D Shapes";

#define BUFFER_OFFSET(offset) ((GLvoid*)offset)

/*
 * Linux:
 * g++ lecture02.cpp -o lecture02APP -lGLEW -lglut -lGLU -lGL
 * Mac:
 * g++ lecture02.cpp -o lecture02APP -lGLEW -framework GLUT -framework OpenGL -framework Cocoa
 */

////2. Load 绘制所需的資料 / 建立儲存資料的資料結構
//三角形顶点坐标数据
/// 1. Generate triangle corners (3 vertices) 2. Store 3 vertices into an array
/// 2. Store 3 vertices into an arra
// generate 3 triangle vertices + store in array
GLfloat afVertex[][3]=
{
	{-0.75,-0.5,0.0},
	{0.75,-0.5,0.0},
	{0.0,0.75,0.0}
};
 
// Vertex Shader
const char* cpVShader = { 
	"#version 330 core\n" 
	"" 
	"in vec3 vPos;" 	// 变量輸入：接受 三個float 的 Vertex Attribute 「Vpos」
	"" 
	"void main() {" 
	"  gl_Position = vec4(vPos,1);"	// 内建变量：4個float 的 Vector （+1 homogenious coordinate）
	"}" 
}; 
 
// Fragment Shader
const char* cpFShader = { 
	"#version 330 core\n"
	"" 
	"out vec4 fColor;" 	// 输出变量名：fColor
	"" 
	"void main() {" 
	"  fColor = vec4( 1, 1, 1, 1 );" 	// 白色
	"}" 
};
 
 
GLuint uVbo;
GLuint uPos;
GLuint uProgram;
 
bool AttachProgram( GLuint uProgram, const char* cpShader, GLenum eShaderType )
{
	GLuint  uShader;
	GLint   iCompleted;
 
	////编译着色器
	uShader = glCreateShader( eShaderType ); 
	glShaderSource( uShader, 1, &cpShader, NULL ); 
	glCompileShader( uShader ); 
	glGetShaderiv( uShader, GL_COMPILE_STATUS, &iCompleted ); 
 
	if ( !iCompleted ) 
	{ 
		GLint  len; 
		char*  msg; 
		glGetShaderiv( uShader, GL_INFO_LOG_LENGTH, &len ); 
		msg = (char*) malloc( len ); 
		glGetShaderInfoLog( uShader, len, &len, msg ); 
		fprintf( stderr, "Vertex shader compilation failure:\n%s\n", msg ); 
		free( msg ); 
		return false;
	}else
	{
		glAttachShader( uProgram, uShader ); 
		return true;
	}
}
 
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 

/// 3. Create GPU Buffer for Vertices 4. Move points GPU memory
/// 4. Move points GPU memory
void initGPUBuffers()
{	
	// Set background color, clear color, etc
	glClearColor( 0.0, 0.0, 1.0, 1.0 ); 	// 設定OpenGL Canvas 背景顏色

	// Generate points to be drawn
	// -- 3. Create GPU Buffer for Vertices -- 
	//// VAO
	GLuint vao;
	glGenVertexArrays( 1, &vao );  // create VAO
	glBindVertexArray( vao );      // make VAO active
	//// VBO
	// 1) Create VBO and give it name (unique ID number)
	glGenBuffers(1, &uVbo);					// 使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
	// 2) Make created VBO currently active one
	glBindBuffer(GL_ARRAY_BUFFER,uVbo);		// 使用glBindBuffer函数把新创建的缓冲VBO绑定到GL_ARRAY_BUFFER目标上
	// -- 4. Move points GPU memory -- 
	// 3) Move points generated earlier to VBO 
	glBufferData(GL_ARRAY_BUFFER,sizeof(afVertex), afVertex, GL_STATIC_DRAW );	// 把之前定义的顶点数据复制到缓冲的内存中
 
	// Initialize shader stuff
	// uProgram = glCreateProgram();			// 產生一個 Shader Program
	// AttachProgram( uProgram, cpVShader , GL_VERTEX_SHADER );	// 然後把Vertex Shader attach至program上
	// AttachProgram( uProgram, cpFShader , GL_FRAGMENT_SHADER );	// 然後把Fragment Shader attach至program上
	// glLinkProgram( uProgram ); 				//最後 Link 起來可以在 Draw時使用這個 Shader Program
	
	// uPos = glGetAttribLocation( uProgram, "vPos" ); 			//取得Shader Program裡面 vPos变量的位置
 
} 
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
////4. Draw and Display
void display() 
{ 
	glClear( GL_COLOR_BUFFER_BIT ); 
		 
	// glUseProgram(uProgram); 				// 設定現在使用的 Shader Program為 uProgram
	// glBindBuffer( GL_ARRAY_BUFFER, uVbo ); 	// 設定現在使用的 GL_ARRAY_BUFFER 為uVbo
	// 設定 GL_ARRAY_BUFFER 裡面 Vertex 資料的格式，並且設定資料是當作 Shader Program 內 uPos 位置的變數內容
	glVertexAttribPointer( uPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) ); // (void*)0 ); //
	glEnableVertexAttribArray( uPos ); 		// Enable uPos 這個 Vertex attribute array
	glDrawArrays( GL_LINE_LOOP, 0, 3 ); 
	// glFlush();
	glutSwapBuffers();					 	// Swap the front and back frame buffers (double buffering)
} 
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
 
void reshape( int width, int height ) 
{ 
	glViewport( 0, 0, width, height ); 
	
	// Compute aspect ratio of the new window
    // if (height == 0) height = 1;                // To prevent divide by 0
    // GLfloat aspect = (GLfloat)width / (GLfloat)height; 

    // // Set the viewport to cover the new window
    // glViewport(0, 0, width, height); 

    // // Set the aspect ratio of the clipping volume to match the viewport
    // glMatrixMode(GL_PROJECTION);                // To operate on the Projection matrix
    // glLoadIdentity();                           // Reset 

    // // Enable perspective projection wiht fovy, aspect, zNear and zFar 
    // gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
} 
 
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
 
void keyboard( unsigned char key, int x, int y ) 
{ 
	  switch( key ) { 
 		case 033: // Escape Key 
			 exit( EXIT_SUCCESS ); 
			 break; 
		} 		 
		glutPostRedisplay(); 
} 
 
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
 
int main( int argc, char* argv[] ) 
{ 
	// GLUT: Opening a window
	glutInit( &argc, argv ); 			// Initializes GLUT
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); 
										// sets display mode (e.g. single framebuffer with RGB colors)
    glutInitWindowSize(640, 480);       // sets window size (Width x Height) in pixels
    glutInitWindowPosition(100, 150);	// sets location of upper left corner (h, v) of window
	glutCreateWindow( argv[0] ); 		// glutCreateWindow("title"); 
	// Inialialize GLEW					// Create window with the given title 
	glewInit(); 						// Then also initialize GLEW
	// GLenum status = glewInit();
    // if (GLEW_OK != status) {
        
    //     printf("GLEW Error:%s\n",glewGetErrorString(status));
    //     return 1;
        
    // }

	// Register input callback functions (GLUT)
	glutDisplayFunc(display); 			// Register callback handler for window Render: re-paint event
	glutReshapeFunc(reshape); 			// Register callback handler for window re-size event						   
	glutKeyboardFunc(keyboard); 		// Register callback handler for input: keyboard event

	glewInit();

	// My initialization 
	// initGL(); 
	// generateGeometry();
	initGPUBuffers();


	// GLUT main loop
	glutMainLoop(); 					// Waits here infinitely till event

	return 0;
} 
