
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
 * g++ VBO_VAO_Square_EBO.cpp -o VBO_VAO_Square_EBO_App -lGLEW -lglut -lGLU -lGL
 * Mac:
 * g++ VBO_VAO_Square_EBO.cpp -o VBO_VAO_Square_EBO_App -lGLEW -framework GLUT -framework OpenGL -framework Cocoa
 */

void MyInit();
void reshape(int w, int h);		//重绘
void display();

GLuint vboId;	//vertex buffer object句柄
GLuint vaoId;	//vertext array object句柄

GLuint eboId;//element buffer object句柄   

//三角形顶点坐标数据
const GLfloat vertices[] = {
	-0.5f,-0.5f,0.0f,1.0f,
	0.5f,-0.5f,0.0f,1.0f,
	0.5f,0.5f,0.0f,1.0f,
	-0.5f,0.5f,0.0f,1.0f,
};

// 索引数据
// GLshort indices[] = {
// 	0, 1, 3,  // 第一个三角形
// 	1, 2, 3   // 第二个三角形
// };

GLshort indices[] = {
	0, 1, 2, 3// 顶点索引
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
	
	//自定义初始化，生成VAO，VBO, EBO
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

	//创建VBO对象，把顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);	//通过分配的ID绑定（bind）一下制定的VBO，对于同一类型的顶点数据一次只能绑定一个VBO
	//传入VBO数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//调用glBufferData把用户定义的数据传输到当前绑定的显存缓冲区中

	//创建EBO对象
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
		//// EBO中存储的内容就是顶点位置的索引indices，EBO跟VBO类似，
		//// 也是在显存中的一块内存缓冲器，只不过EBO保存的是顶点的索引。
	//传入EBO数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//解释顶点数据方式
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	//顶点数据传入GPU之后，还需要通知OpenGL如何解释这些顶点数据
		// 第一个参数指定顶点属性位置，与顶点着色器中layout(location=0)对应。
		// 第二个参数指定顶点属性大小。
		// 第三个参数指定数据类型。
		// 第四个参数定义是否希望数据被标准化。
		// 第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔。
		// 第六个参数表示我们的位置数据在缓冲区起始位置的偏移量。
		//// 顶点属性glVertexAttribPointer默认是关闭的，
		//// 使用时要以顶点属性位置值为参数调用glEnableVertexAttribArray开启。如
	glEnableVertexAttribArray(0);
	
	//解绑VAO
	glBindVertexArray(0);
	//解绑EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	//绑定VAO
	glBindVertexArray(vaoId);
=
	//绑定VBO
	// glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// glEnableVertexAttribArray(0);

	
	//绘制模型: OpenGL中所有的图形都是通过分解成三角形的方式进行绘制，
	//glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertices));
		// glDrawArrays (GLenum mode, GLint first, GLsizei count)
		//// glDrawArrays函数负责把模型绘制出来， 它使用当前激活的着色器，
		//// 当前VAO对象中的VBO顶点数据和属性配置来绘制出来基本图形。
	

	//绘制模型: 当用EBO绑定顶点索引的方式绘制模型时，需要使用glDrawElements而不是glDrawArrays
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, NULL);
		// 第一个参数指定了要绘制的模式；
		// 第二个参数指定要绘制的顶点个数；
		// 第三个参数是索引的数据类型；
		// 第四个参数是可选的EBO中偏移量设定。

	//解除VBO绑定
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

