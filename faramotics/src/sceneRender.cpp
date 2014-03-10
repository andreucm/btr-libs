#include "sceneRender.h"

CsceneRender::CsceneRender(bool visible)
{
	//glutIsInit = false; //this variable should be shared by all instances to this object
	isVisible = visible;
	viewPoint.setPose(1,5,1,0,0,0); //initializes view point
	modelGlm = 0; // set null pointers	
	//modelList = glGenLists(1);
	//cout << "CsceneRender::CsceneRender(void): End of constructor" << endl;
}

CsceneRender::CsceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ, bool visible)
{
	//glutIsInit = false; //this variable should be shared by all instances to this object
	isVisible = visible; 
	viewPoint.setPose(1,5,1,0,0,0); //initializes view point
	modelGlm = 0; // set null pointers	

	//init window and GL state
      //initGLUT(); //to do: to be coordinated with boolean glutIsInit ...
	setRenderParameters(ww, hh, hAp, vAp, nearZ, farZ);
	initWindow();
	initGL();
      
      //init  render lists
      modelList = glGenLists(1);

      //if (!isVisible) this->hide();
	//cout << "CsceneRender::CsceneRender(,,,,,): End of constructor" << endl;
}

CsceneRender::~CsceneRender()
{
	glutDestroyWindow(winId);
	
      if ( modelGlm != 0 )
	{
		glmDelete(modelGlm);
		//cout << "CsceneRender::~CsceneRender(): Deleting 3d model" << endl;
	}
	
	/*if ( scene != NULL )
      {
            aiReleaseImport(scene);
      }*/
      
	//cout << "CsceneRender::~CsceneRender(): End of destructor" << endl;
}

// void CsceneRender::initGLUT(int argc, char** argv)
// {
// 	//initialize GLUT window
// 	if (!glutIsInit)
// 	{
// 		glutInit(&argc, argv);
// 		glutIsInit = true;
// 	}
// }

void CsceneRender::initWindow()
{
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (widthP, heightP);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("sceneRender");
	winId = glutGetWindow();
	if(!isVisible) glutHideWindow();
}

void CsceneRender::initGL()
{
	//initialize GL
	glClearColor(0.0, 0.0, 0.8, 0.5); //sets background color to blue
	glClearDepth(1.0); //Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); //enables hidden-surface removal
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //renderizes both faces of polygons	

	//sets projection and viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vAperture*180.0/M_PI, widthM/heightM, zNear, zFar);
	glViewport(0, 0, widthP, heightP);
	glFinish();
}

void CsceneRender::show()
{
	isVisible = true;
	glutSetWindow(winId);
	glutShowWindow();	
	render();
}

void CsceneRender::hide()
{
	isVisible = false;
	glutSetWindow(winId);
	glutHideWindow();
}

void CsceneRender::fullScreen()
{
	//to do: code below doesn't work properly. why??

	glutSetWindow(winId);
	
	widthP = glutGet(GLUT_SCREEN_WIDTH);
	heightP = glutGet(GLUT_SCREEN_HEIGHT);
	cout << "sW = " << widthP << "; sH = " << heightP << endl; 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vAperture*180.0/M_PI, widthM/heightM, zNear, zFar);
	glViewport(0, 0, widthP, heightP);
	
	glFinish();
	glutFullScreen();
}

void CsceneRender::setRenderParameters(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ)
{
	widthP = ww;
	heightP = hh;
	hAperture = hAp;
	vAperture = vAp;
	zNear = nearZ;
	zFar = farZ;
	widthM = 2.0*zNear*tan(hAperture/2.0); //metric width
	heightM = 2.0*zNear*tan(vAperture/2.0); //metric height	
}

void CsceneRender::printRenderParameters()
{
	cout << "CsceneRender::printRenderParameters:" << endl;
	cout << "  w [pixels] = " << widthP << endl;
	cout << "  h [pixels] = " << heightP << endl;
	cout << "  apertureH [deg] = " << hAperture*180.0/M_PI << endl;
	cout << "  apertureV [deg] = " << vAperture*180.0/M_PI << endl;
	cout << "  w [meters] = " << widthM << endl;
	cout << "  h [meters] = " << heightM << endl;
	cout << "  zNear [meters] = " << zNear << endl;
	cout << "  zFar [meters] = " << zFar << endl;
}

void CsceneRender::setViewPoint(Cpose3d & vP)
{
	viewPoint.setPose(vP);
}

void CsceneRender::setViewPoint(double cx, double cy, double cz, double ax, double ay, double az, bool rd)
{
	viewPoint.setPose(cx,cy,cz,ax,ay,az,rd);
}

void CsceneRender::render()
{
   	lookAtValues lav;
      
      std::cout << "START render()" << std::endl;
	
	glutSetWindow(winId);
	if (!isVisible) this->hide();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer with previous values
	glMatrixMode(GL_MODELVIEW); //necessary to perform viewing and model transformations
	glLoadIdentity(); //resets the model/view matrix
	viewPoint.getLookAt(lav); //gets look at values from a position expressed as (x,y,z,h,p,r)
	gluLookAt(lav.ex,lav.ey,lav.ez,lav.ax,lav.ay,lav.az,lav.ux,lav.uy,lav.uz);
	glCallList(modelList);
      glutSwapBuffers();//To be  confirmed what it's doing !!!
	glFinish();
	//if (!isVisible) this->hide();
	//viewPoint.printPose();
	//cout << lav.ex << " " << lav.ey << " " << lav.ez << " " << lav.ax << " " << lav.ay << " " << lav.az << " " << lav.ux << " " << lav.uy << " " << lav.uz << endl;
      
      std::cout << "END render()" << std::endl;
}

int CsceneRender::loadAssimpModel(const string & modelFile)
{      
      std::cout << "START loadAssimpModel()" << std::endl;
      
      // Import model file
      const struct aiScene* scene = importer.ReadFile( modelFile, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
      
      // If the import failed, report it
      if ( !scene )
      {
            std::cout << importer.GetErrorString() << std::endl;
            return -1;
      }
      else //if scene != NULL, import suceed, so associates the scene to the model List
      {
            std::cout << "scene->mRootNode.mNumMeshes: " << scene->mRootNode->mNumMeshes << std::endl;      
            glutSetWindow(winId);
            glNewList(modelList, GL_COMPILE);      
            this->recursive_render(scene, scene->mRootNode);
            glEndList();
            glFinish(); //finish all openGL work      
            
            // We're done. Everything will be cleaned up by the importer destructor
            std::cout << "END loadAssimpModel()" << std::endl;
            return 1;
      }
}

int CsceneRender::loadModel(const string & modelFile, const string & edgesFile)
{
	glutSetWindow(winId);
	glNewList(modelList, GL_COMPILE);

	//polygonal model
	modelGlm = glmReadOBJ(modelFile.data(), 0., 0., 0.);
	if ( modelGlm == NULL )
	{
		//cout << " CsceneRender: " << __LINE__ << endl;
		return -1;
	}
	glColor3f( .8, .8, .8);
	//glmDraw(modelGlm, GLM_TEXTURE);//tested with GLM_NONE and same performance has been obtained in expected scan computation. May be the model hadn't texture ... ???
	//glmDraw(modelGlm, GLM_NONE);
	glmDraw(modelGlm, GLM_COLOR);
	
	//edges
	if (edgesFile.compare("") != 0) //edgesFile contains something
	{
		edges = glmReadOBJ(edgesFile.data(), 0., 0., 0.);
		glColor3f( .0, .0, .0);
		glLineWidth( 2.);
		//glmDraw(edges, GLM_TEXTURE);
		//glmDraw(edges, GLM_NONE);
		glmDraw(edges, GLM_COLOR);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
	}
	
	glEndList();
	glFinish();	//finish all openGL work
	return 1;
}

void CsceneRender::loadModel(const int modelID)
{
	glutSetWindow(winId);
	//modelList = glGenLists(1);
	glNewList(modelList, GL_COMPILE);

	switch(modelID)
	{
		case DEBUG_SCENE:
                  //simple rectangular floor
                  glColor3f(0.0,1.0,0.7);
                  glBegin(GL_QUADS);//Draws the floor
                        glNormal3f( 0.0,0.0, 1.0);
                        glVertex3f(-1.0,23.0, 0.0);
                        glVertex3f(-1.0,-1.0, 0.0);
                        glVertex3f( 23.0,-1.0, 0.0);
                        glVertex3f( 23.0, 23.0, 0.0);
                  glEnd();

                  //draws a green wall
                  glColor3f( 0.0, 1.0, 0.0);
                  glBegin(GL_QUADS);
                        glNormal3f( -1.0, 0.0, 0.0);
                        glVertex3f(15.0, 15.0, 0.0);
                        glVertex3f(15.0, 7.0, 0.0);
                        glVertex3f(15.0, 7.0, 3.0);
                        glVertex3f(15.0, 15.0, 3.0);
                  glEnd();

                  //Draws a magenta wall
                  glColor3f( 1., 0., 1.0);
                  glBegin(GL_QUADS);
                        glNormal3f( -1.0, 0.0, 0.0);
                        glVertex3f(14.0, 17.0, 0.0);
                        glVertex3f(14.0, 12.0, 0.0);
                        glVertex3f(14.0, 12.0, 2.0);
                        glVertex3f(14.0, 17.0, 2.0);
                  glEnd();

                  //draws an orange cone
                  glColor3f(0.9,0.6,0.0);
                  glTranslatef(12.0,15.5,0.0);// moves model origin
                  gluCylinder(gluNewQuadric(),1.2, 0.0, 3.5, 25,25);
                  glTranslatef(-12.0,-15.5,0.0);// moves model origin

                  //draws a yellow sphere
                  glColor3f(1,0.9,0.1);
                  glTranslatef(11.0,10.0,0.0);// moves model origin
                  gluSphere(gluNewQuadric(),2.0, 40,40);
                  glTranslatef(-11.0,-10.0,0.0);// moves model origin
                  break;
                  
		case SPHERE:
                  //draws a yellow sphere
                  glColor3f(1,0.9,0.1);
                  glTranslatef(3.0,0.0,0.0);// moves model origin
                  gluSphere(gluNewQuadric(),2.0, 140,140);
                  //glTranslatef(-3.0,0.0,0.0);// comes back
                  break;

		case FME_BIG_DOOR: //hard coded map of the "IRI big iron door"
			glColor3f(0.0,1.0,0.7);
			glBegin(GL_QUADS);//Draws the floor
				glNormal3f( 0.0,0.0, 1.0);
				glVertex3f(-1.0,23.0, 0.0);
				glVertex3f(-1.0,-1.0, 0.0);
				glVertex3f( 23.0,-1.0, 0.0);
				glVertex3f( 23.0, 23.0, 0.0);
			glEnd();

			glColor3f( 0.9, 0.3, 0.0); //draws a wall
			glBegin(GL_QUADS);
				glVertex3f(0.0,0.0,0.0);
				glVertex3f(0.0,20.0,0.0);
				glVertex3f(0.0,20.0,3.0);
				glVertex3f(0.0,0.0,3.0);
			glEnd();

			glColor3f( 1.0, 0.5, 0.0);
			glBegin(GL_QUADS); //the big door closed!
				glVertex3f(4.0,4.65,0.0);
				glVertex3f(4.0,4.65,3.0);
				glVertex3f(14.0,4.65,3.0);
				glVertex3f(14.0,4.65,0.0);
			glEnd();
			
			glColor3f( 1.0, 0.5, 0.0);
			glBegin(GL_QUAD_STRIP);
				glVertex3f(0.0,3.8,0.0);
				glVertex3f(0.0,3.8,3.0);
				glVertex3f(4.0,3.8,0.0);
				glVertex3f(4.0,3.8,3.0);
				glVertex3f(4.0,4.65,0.0);
				glVertex3f(4.0,4.65,3.0);
				glVertex3f(3.4,4.65,0.0);
				glVertex3f(3.4,4.65,3.0);
				glVertex3f(3.4,13.85,0.0);
				glVertex3f(3.4,13.85,3.0);
				glVertex3f(3.17,13.85,0.0);
				glVertex3f(3.17,13.85,3.0);
				glVertex3f(3.17,16.68,0.0);
				glVertex3f(3.17,16.68,3.0);
				glVertex3f(3.74,16.68,0.0);
				glVertex3f(3.74,16.68,3.0);
				glVertex3f(3.74,17.4,0.0);
				glVertex3f(3.74,17.4,3.0);
				glVertex3f(0.0,17.4,0.0);
				glVertex3f(0.0,17.4,3.0);
			glEnd();

			glBegin(GL_QUAD_STRIP); //stairs
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,21.3,0.0);
				glVertex3f(0.0,21.3,0.0);
				glVertex3f(18.0,21.3,0.2);
				glVertex3f(0.0,21.3,0.2);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,21.5,0.2);
				glVertex3f(0.0,21.5,0.2);
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,21.5,0.4);
				glVertex3f(0.0,21.5,0.4);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,21.7,0.4);
				glVertex3f(0.0,21.7,0.4);
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,21.7,0.6);
				glVertex3f(0.0,21.7,0.6);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,21.9,0.6);
				glVertex3f(0.0,21.9,0.6);
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,21.9,0.8);
				glVertex3f(0.0,21.9,0.8);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,22.1,0.8);
				glVertex3f(0.0,22.1,0.8);
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,22.1,1.0);
				glVertex3f(0.0,22.1,1.0);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,22.3,1.0);
				glVertex3f(0.0,22.3,1.0);
				glColor3f( 1.0,0.5,0.0);
				glVertex3f(18.0,22.3,1.2);
				glVertex3f(0.0,22.3,1.2);
				glColor3f( 1.0,0.7,0.0);
				glVertex3f(18.0,22.5,1.2);
				glVertex3f(0.0,22.5,1.2);	
			glEnd();

			glColor3f( 1.0, 0.5, 0.0);
			glBegin(GL_QUAD_STRIP);
				glVertex3f(18.0,17.4,0.0);
				glVertex3f(18.0,17.4,3.0);
				glVertex3f(14.0,17.4,0.0);
				glVertex3f(14.0,17.4,3.0);
				glVertex3f(14.0,16.68,0.0);
				glVertex3f(14.0,16.68,3.0);
				glVertex3f(14.57,16.68,0.0);
				glVertex3f(14.57,16.68,3.0);
				glVertex3f(14.57,13.85,0.0);
				glVertex3f(14.57,13.85,3.0);
				glVertex3f(14.34,13.85,0.0);
				glVertex3f(14.34,13.85,3.0);
				glVertex3f(14.34,4.65,0.0);
				glVertex3f(14.34,4.65,3.0);
				glVertex3f(13.74,4.65,0.0);
				glVertex3f(13.74,4.65,3.0);
				glVertex3f(13.74,3.8,0.0);
				glVertex3f(13.74,3.8,3.0);
				glVertex3f(18.0,3.8,0.0);
				glVertex3f(18.0,3.8,3.0);
			glEnd();

			//marks the orgin with a red stick
			glColor3f( 1.0, 0.0, 0.0);
			glDisable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
				glVertex3f(0.0,0.0,0.0);
				glVertex3f(0.0,0.0,2.0);
			glEnd();
			break; //break case
			
		default:
			cout << "CsceneRender::loadModel(): Unknown modelID" << endl; 
			break;
	}
	//glLoadIdentity();
	glEndList();
	glFinish();	//finish all openGL work
}


//assimp functions 
//this recursive_render has been found here: http://www.lighthouse3d.com/cg-topics/code-samples/importing-3d-models-with-assimp/
/*void recursive_render (const aiScene *sc, const aiNode* nd)
{
 
    // Get node transformation matrix
    aiMatrix4x4 m = nd->mTransformation;
    // OpenGL matrices are column major
    m.Transpose();
 
    // save model matrix and apply node transformation
    pushMatrix();
 
    float aux[16];
    memcpy(aux,&m,sizeof(float) * 16);
    multMatrix(modelMatrix, aux);
    setModelMatrix();
 
    // draw all meshes assigned to this node
    for (unsigned int n=0; n < nd->mNumMeshes; ++n){
        // bind material uniform
        glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial)); 
        // bind texture
        glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
        // bind VAO
        glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
        // draw
        glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0);
 
    }
 
    // draw all children
    for (unsigned int n=0; n < nd->mNumChildren; ++n){
        recursive_render(sc, nd->mChildren[n]);
    }
    popMatrix();
}*/

void CsceneRender::recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{
      unsigned int i;
      unsigned int n = 0, t;
      aiMatrix4x4 m = nd->mTransformation;

      // update transform
      aiTransposeMatrix4(&m);
      glPushMatrix();
      glMultMatrixf((float*)&m);

      // draw all meshes assigned to this node
      std::cout << __LINE__ << ": nd->mNumMeshes: " << nd->mNumMeshes << std::endl;
      //std::cout << __LINE__ << ": sc->mMeshes[nd->mMeshes[n]]: " << sc->mMeshes[nd->mMeshes[n]] << std::endl; 
      for (; n < nd->mNumMeshes; ++n)
      {
            const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
            //apply_material(sc->mMaterials[mesh->mMaterialIndex]);
            
            if(mesh->mNormals == NULL) 
            {
                  glDisable(GL_LIGHTING);
            } 
            else 
            {
                  glEnable(GL_LIGHTING);
            }

            std::cout << __LINE__ << ": mesh->mNumFaces: " << mesh->mNumFaces << std::endl; 
            
            bool dbg=true;
            if (dbg)
            {
                  std::cout << __LINE__ << ": mesh->HasFaces(): " << mesh->HasFaces() << std::endl;
                  if (mesh->mFaces == NULL) std::cout << __LINE__ << ": mesh->mFaces is NULL" << std::endl; 
                  std::cout << __LINE__ << ": mesh->HasPositions(): " << mesh->HasPositions() << std::endl; 
                  std::cout << __LINE__ << ": mesh->HasNormals(): " << mesh->HasNormals() << std::endl; 
            }
            
            for (t = 0; t < mesh->mNumFaces; ++t) 
            {
                  std::cout << __LINE__ << ": t: " << t << std::endl; 
                  const struct aiFace* face = &mesh->mFaces[t];
                  GLenum face_mode;
                  std::cout << __LINE__ << ": face(ptr): " << face << endl;
                  std::cout << __LINE__ << ": mesh->mFaces[t].mNumIndices: " << mesh->mFaces[t].mNumIndices << std::endl; 
                  
                  switch(face->mNumIndices) 
                  {
                        case 1: face_mode = GL_POINTS; std::cout << "GL_POINTS" << std::endl; break;
                        case 2: face_mode = GL_LINES; std::cout << "GL_LINES" << std::endl; break;
                        case 3: face_mode = GL_TRIANGLES; std::cout << "GL_TRIANGLES" << std::endl; break;
                        default: face_mode = GL_POLYGON; std::cout << "GL_POLYGON" << std::endl; break;
                  }
                  std::cout << __LINE__ << ": t: " << t << std::endl; 

                  glBegin(face_mode);
                  for(i = 0; i < face->mNumIndices; i++) 
                  {
                        std::cout << __LINE__ << ": i: " << i << std::endl; 
                        int index = face->mIndices[i];
                        if(mesh->mColors[0] != NULL) glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                        if(mesh->mNormals != NULL) glNormal3fv(&mesh->mNormals[index].x);
                        glVertex3fv(&mesh->mVertices[index].x);
                  }
                  glEnd();
            }
      }

      // draw all children
      for (n = 0; n < nd->mNumChildren; ++n) 
      {
            recursive_render(sc, nd->mChildren[n]);
      }

      // pop gl matrix
      glPopMatrix();
}

void CsceneRender::apply_material(const struct aiMaterial *mtl)
{
      float c[4];

      GLenum fill_mode;
      int ret1, ret2;
      aiColor4D diffuse;
      aiColor4D specular;
      aiColor4D ambient;
      aiColor4D emission;
      float shininess, strength;
      int two_sided;
      int wireframe;
      unsigned int max;

      set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            color4_to_float4(&diffuse, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

      set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
            color4_to_float4(&specular, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

      set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
            color4_to_float4(&ambient, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

      set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
            color4_to_float4(&emission, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

      max = 1;
      ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
      if(ret1 == AI_SUCCESS) {
      max = 1;
      ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
            if(ret2 == AI_SUCCESS)
                  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
      else {
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
            set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
      }

      max = 1;
      if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
            fill_mode = wireframe ? GL_LINE : GL_FILL;
      else
            fill_mode = GL_FILL;
      glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

      max = 1;
      if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
            glDisable(GL_CULL_FACE);
      else 
            glEnable(GL_CULL_FACE);
}

void CsceneRender::color4_to_float4(const aiColor4D *c, float f[4])
{
      f[0] = c->r;
      f[1] = c->g;
      f[2] = c->b;
      f[3] = c->a;
}

void CsceneRender::set_float4(float f[4], float a, float b, float c, float d)
{
      f[0] = a;
      f[1] = b;
      f[2] = c;
      f[3] = d;
}



