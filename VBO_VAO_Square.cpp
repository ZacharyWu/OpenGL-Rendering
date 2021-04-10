
// glew.h
#ifdef __APPLE__
#include</opt/local/include/GL/glew.h>
#else
#include <GL/glew.h>
#endif

// glut.h
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
 * Linux:
 * g++ VBO_VAO_Square.cpp -o VBO_VAO_Square_App -lGLEW -lglut -lGLU -lGL
 * Mac:
 * g++ VBO_VAO_Square.cpp -o VBO_VAO_Square_App -lGLEW -framework GLUT -framework OpenGL -framework Cocoa
 */

void MyInit();
void reshape(int w, int h);		//重绘
void display();

GLuint vboId;	//vertex buffer object句柄
GLuint vaoId;	//vertext array object句柄
// GLuint programId;//shader program 句柄 

//三角形顶点坐标数据
const GLfloat vertices[] = {
	-0.5f,-0.5f,0.0f,1.0f,
	0.5f,-0.5f,0.0f,1.0f,
	0.5f,0.5f,0.0f,1.0f,
	-0.5f,0.5f,0.0f,1.0f,
};

void keyboard( unsigned char key, int x, int y )  //键盘退出事件
{ 
	  switch( key ) { 
 		case 033: // Escape Key 
			 exit( EXIT_SUCCESS ); 
			 break; 
		} 		 
		glutPostRedisplay(); 
} 


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("VBO_VAO_RECTANGLE");
	
	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();
	
	//自定义初始化，生成VAO，VBO对象
	MyInit();

	//重绘函数	
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&display);

	//注册键盘按键退出事件
	glutKeyboardFunc(keyboard); 		// Register callback handler for input: keyboard event
	
	glutMainLoop();
	return 0;
}

void MyInit()
{
	glClearColor(0.0,0.0, 0.0,0.0);

	//创建VAO对象: VAO相当于是对很多个VBO的引用，把一些VBO组合在一起作为一个对象统一管理
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
		// VAO是一个保存了所有顶点数据属性的状态结合，它存储了顶点数据的格式以及顶点数据所需的VBO对象的引用
		//// 执行VAO绑定之后其后的所有VBO配置都是这个VAO对象的一部分，
		//// 可以说VBO是对顶点属性信息的绑定，VAO是对很多个VBO的绑定。

	//创建VBO对象
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);	//通过分配的ID绑定（bind）一下制定的VBO，对于同一类型的顶点数据一次只能绑定一个VBO

	//传入VBO数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//调用glBufferData把用户定义的数据传输到当前绑定的显存缓冲区中

	//解除VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void reshape(int w, int h)
{
	glViewport(0, 0,(GLsizei)w, (GLsizei)h);

	// Compute aspect ratio of the new window
    // if (height == 0) height = 1;                // To prevent divide by 0
    // GLfloat aspect = (GLfloat)width / (GLfloat)height; 

    // Set the viewport to cover the new window
    // glViewport(0, 0, width, height); 

    // Set the aspect ratio of the clipping volume to match the viewport
    // glMatrixMode(GL_PROJECTION);                // To operate on the Projection matrix
    // glLoadIdentity();                           // Reset 

    // // Enable perspective projection wiht fovy, aspect, zNear and zFar 
    // gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);

	//解释顶点数据方式
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	//顶点数据传入GPU之后，还需要通知OpenGL如何解释这些顶点数据
		// 第一个参数指定顶点属性位置，与顶点着色器中layout(location=0)对应。
		// 第二个参数指定顶点属性大小。
		// 第三个参数指定数据类型。
		// 第四个参数定义是否希望数据被标准化。
		// 第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔。
		// 第六个参数表示我们的位置数据在缓冲区起始位置的偏移量。
		//// 顶点属性glVertexAttribPointer默认是关闭的，
		//// 使用时要以顶点属性位置值为参数调用glEnableVertexAttribArray开启。如glEnableVertexAttribArray(0)
	
	//绘制模型: OpenGL中所有的图形都是通过分解成三角形的方式进行绘制，
	glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertices));
		// glDrawArrays (GLenum mode, GLint first, GLsizei count)
		// 第一个参数mode表示绘制的类型，有三种取值：
		// 1.GL_TRIANGLES：每三个顶之间绘制三角形，之间不连接；
		// 2.GL_TRIANGLE_FAN：以V0V1V2,V0V2V3,V0V3V4，……的形式绘制三角形；
		// 3.GL_TRIANGLE_STRIP：顺序在每三个顶点之间均绘制三角形。这个方法可以保证从相同的方向上所有三角形均被绘制。以V0V1V2,V1V2V3,V2V3V4……的形式绘制三角形；
		// 第二个参数定义从缓存中的哪一位开始绘制，一般定义为0；
		// 第三个参数定义绘制的顶点数量
		//// glDrawArrays函数负责把模型绘制出来， 它使用当前激活的着色器，
		//// 当前VAO对象中的VBO顶点数据和属性配置来绘制出来基本图形。

	//解除VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

