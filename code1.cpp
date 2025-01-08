#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <cmath>
#include <cstdio>

// Variabel tekstur dan id tia teksturnya
GLuint texture_Apel_ID, texture_Jeruk_ID, texture_Keranjang_ID;

// Variabel rotasi global
float rotationAngle = 0.0f;
bool isRotating = false; // Status rotasi nonaktif

// Variabel untuk translasi dan skala
float translateY = 0.0f;
float translateX = 0.0f; 
float scaleFactor = 1.0f;

// Variabel untuk garis kartesius
bool showCartesius = true; // Status garis Kartesius aktif


// Fungsi untuk membuat bola (buah)
void createSphere(float radius) {
    GLUquadric* sphere = gluNewQuadric();
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, radius, 100, 100);
}

// Fungsi untuk membuat keranjang
void createBasketWithHandle() {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    // Dasar keranjang berbentuk piring
    glPushMatrix();
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluDisk(quadric, 0.0, 2.5, 100, 1);
    glPopMatrix();

    // Dinding keranjang
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadric, 3.5, 2.5, 2.0, 100, 100);
    glPopMatrix();
}

// Fungsi untuk membuat meja
void createTable() {
    glColor3f(0.5f, 0.35f, 0.05f); // Warna coklat untuk meja
    glPushMatrix();
    glScalef(6.0f, 0.3f, 6.0f);    // Skala untuk membuat meja lebih lebar
    glutSolidCube(1.0f);           // Meja berbentuk kubus
    glPopMatrix();

    // Tambahkan kaki meja
    glPushMatrix();
    glTranslatef(2.5f, -1.65f, 2.5f); // Kaki kanan depan
    glScalef(0.3f, 3.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.5f, -1.65f, 2.5f); // Kaki kiri depan
    glScalef(0.3f, 3.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.5f, -1.65f, -2.5f); // Kaki kanan belakang
    glScalef(0.3f, 3.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.5f, -1.65f, -2.5f); // Kaki kiri belakang
    glScalef(0.3f, 3.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCartesius() {
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk garis
    glBegin(GL_LINES);

    // Sumbu X
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    // Sumbu Y
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    // Sumbu Z
    glVertex3f(0.0f, 0.0f, -10.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);

    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 15.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);

    // Rotasi seluruh objek jika diaktifkan
    if (isRotating) {
        glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    }

    // Translasi dan skala seluruh objek
    glTranslatef(translateX, translateY, 0.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Gambar garis Kartesius jika diaktifkan
    if (showCartesius) {
//        glPushMatrix();
//        glTranslatef(0.0, -0.7, 0.0); // posisi keranjang
        drawCartesius();
//        glPopMatrix();
	}
	
    // Objek (meja, keranjang, buah)
    glPushMatrix();
    glTranslatef(0.0f, -3.0f, 0.0f); // Posisi meja
    createTable();
    glPopMatrix();
	
	glEnable(GL_LIGHTING); //hidupkan cahaya
	
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0, -0.7, 0.0); // Posisi keranjang
    glBindTexture(GL_TEXTURE_2D, texture_Keranjang_ID);
    createBasketWithHandle();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8, -1.8, 0.0); // Posisi apel
    glBindTexture(GL_TEXTURE_2D, texture_Apel_ID);
    createSphere(0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8, -1.8, 0.0); // Posisi jeruk
    glBindTexture(GL_TEXTURE_2D, texture_Jeruk_ID);
    createSphere(0.8);
    glPopMatrix();

    glDisable(GL_LIGHTING); // nonaktifan cahaya
    glutSwapBuffers();
}


// Fungsi untuk memuat tekstur
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    void* imgData;
    int imgWidth, imgHeight;

    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return 0;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return 0;
    }

    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);

    if (imgData) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        FreeImage_Unload(bitmap2);
    } else {
        printf("Failed to get texture data from %s\n", path);
        FreeImage_Unload(bitmap2);
    }

    return textureID;
}

// Fungsi timer untuk animasi rotasi
void updateRotation(int value) {
    if (isRotating) {
        rotationAngle += 0.5f;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateRotation, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r': // Toggle rotasi
            isRotating = !isRotating;
            break;
        case 'w': // Translasi ke atas
            translateY += 0.1f;
            break;
        case 'z': // Translasi ke bawah
            translateY -= 0.1f;
            break;
        case 'a': // Translasi ke kiri
            translateX -= 0.1f;
            break;
        case 's': // Translasi ke kanan
            translateX += 0.1f;
            break;
        case '+': // Perbesar skala
            scaleFactor += 0.1f;
            break;
        case '-': // Perkecil skala
            scaleFactor -= 0.1f;
            break;
        case 'c': // Toggle garis Kartesius
            showCartesius = !showCartesius;
            break;
        case 27: // Tombol Esc untuk keluar
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void initProjection() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Inisialisasi proyeksi
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 16.0 / 9.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Konfigurasi pencahayaan
    glEnable(GL_LIGHT0); // Aktifkan lampu sumber cahaya pertama
    GLfloat lightAmbient[] = { 2.2f, 2.2f, 2.2f, 2.2f }; // Cahaya lingkungan (ambient)
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Cahaya tersebar (diffuse)
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 2.0f }; // Cahaya spesular (highlight)
    GLfloat lightPosition[] = { 5.0f, 10.0f, 10.0f, 1.0f }; // Posisi sumber cahaya

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Memuat tekstur
    texture_Apel_ID = loadTexture("textures/apel.png");
    texture_Jeruk_ID = loadTexture("textures/jeruk.png");
    texture_Keranjang_ID = loadTexture("textures/keranjang.png");
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Buah-Buahan Kelompok 9");

    glewInit();
    initProjection();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updateRotation, 0);
    glutMainLoop();
    return 0;
}

