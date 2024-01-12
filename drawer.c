
#include "header.h"
#include <GL/glut.h>
#include "shared_memories.h"
#include "customer_header.h"
#include "shelving_team.h"
#include "functions.h"
#include "processing_times.h"

unsigned int read_queue_size();

void sigterm_handler2() ;


// functions for openGl
void display(void);
void reshape(int w, int h);
void timer(int z);
void drawReceptionHall();
void drawEnterOfQueues();
void drawQueue();
void drawWattingHall();
void drawMetalDetector();
void drawChairs();
void drawTellers();
void drawText();
void renderText(int, int, void *, char *);
void readDataFromSharedMemory();
void drawCashier1();
void drawCashier2();
void drawCashier3();


int customer_count = 0;
int cashier1_totalCount = 0;
int  cashier3_totalCount = 0;
int cashier2_totalCnt=0;

Product items_count[4];

int milk_count = 0;
int eggs_count = 0;
int bread_count = 0;
int apples_count = 0;

int milk_in_shelf = 0;
int eggs_in_shelf = 0;
int bread_in_shelf = 0;
int apples_in_shelf = 0;


int fram_rate = 1900 / 2;

float mouseX = 0.0f;
float mouseY = 0.0f;
int windowWidth = 1800;
int windowHeight = 1000;


char *drawer_current_time = "";


void mouseMotion(int x, int y) {
    // Convert pixel coordinates (x, y) to OpenGL coordinates
    mouseX = (x / (float)windowWidth) * 40.0f - 20.0f;
    mouseY = ((windowHeight - y) / (float)windowHeight) * 40.0f - 20.0f;
    glutPostRedisplay(); // Add this line to ensure the display function is called to update the rendering
}



int main(int argc, char **argv)
{

    read_items("supermarket_items.txt");
    c = read_config("config.txt");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 1000);
    glutCreateWindow("Super Market");
    glLineWidth(5);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
void display(void) {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // All your drawing operations.
    drawReceptionHall();
    drawEnterOfQueues();
    drawQueue();
    drawWattingHall();
    drawMetalDetector();
    drawChairs();
    drawCashier1();
    drawCashier2();
    drawCashier3();
    drawTellers();
    drawText();

    // Render the mouse coordinates
    char mouseCoords[50];
    sprintf(mouseCoords, "Mouse X: %.2f Y: %.2f", mouseX, mouseY);
    renderText(10, -18, GLUT_BITMAP_TIMES_ROMAN_24, mouseCoords);

    // Swap the buffers at the very end after all drawing commands
    glutSwapBuffers();
}
void sigterm_handler2() {

    exit(0);
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int z)
{
    glutTimerFunc(fram_rate, timer, 0);
    glutPostRedisplay();
    readDataFromSharedMemory();
}
void drawReceptionHall()
{
    glBegin(GL_LINES);
    glVertex2f(-20.0, -13.0);
    glVertex2f(-10.0, -13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-20.0, 13.0);
    glVertex2f(-10.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, 13.0);
    glVertex2f(-10.0, 11.0);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -13.0);
    glVertex2f(-10.0, -10.0);
    glEnd();
    glFlush();




}
void drawBox(float x, float y, float width, float height) {
    // Top line
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 255);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glEnd();

    // Bottom line
    glBegin(GL_LINES);
    glVertex2f(x, y - height);
    glVertex2f(x + width, y - height);
    glEnd();

    // Left line
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y - height);
    glEnd();

    // Right line
    glBegin(GL_LINES);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glEnd();

    glFlush();
}

void drawEnterOfQueues() // cashier border
{
    float startY = 13.0;
    float totalSpace = 26.0; // startY - (-13.0)
    int numLines = 4;
    float space = totalSpace / (numLines - 1);

    // Calculate y-coordinates for each cashier
    float y1 = startY - 1 * space;
    float y2 = startY - 2 * space;
    float y3 = startY - 3 * space;

    // Adjust box sizes if necessary
    float boxWidth = 2.0;
    float boxHeight = 2.0;

    // Draw box for cashier 1
    drawBox(-7.0, y1 + 2, boxWidth, boxHeight);

    // Draw box for cashier 2
    drawBox(-7.0, y2 + 2, boxWidth, boxHeight);

    // Draw box for cashier 3
    drawBox(-7.0, y3 + 2, boxWidth, boxHeight);
}



void drawQueue() // cashier1 queue,  cashier2 queue , cashier3 queue
{

    float startY = 13.0;   // Starting y-coordinate (upper limit)
    float endY = -13.0;    // Ending y-coordinate (lower limit)
    int numLines = 4;      // Number of lines
    float totalSpace = startY - endY;
    float space = totalSpace / (numLines - 1);

    for (int i = 0; i < numLines; i++) {
        float y = startY - i * space;

        glBegin(GL_LINES);
        glColor3f(0, 255, 0);
        glVertex2f(-10.0, y);
        glVertex2f(-2.0, y);
        glEnd();

        // Drawing points representing cashiers
        if (i > 0) {
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(0, 255, 0);
            glVertex2f(-6.0, y + 1);
            glEnd();
        }
    }

    glFlush();


}


void drawWattingHall()
{
    glColor3ub(255, 0, 255);

    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(-2.0, 11.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(-2.0, -10.0);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(20.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(20.0, -13.0);
    glEnd();
    glFlush();
}
void drawMetalDetector()
{

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 255, 0);
    glVertex2f(4, -13.0);
    glEnd();
    glFlush();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(255, 0, 0);
    glVertex2f(-15, -13.0);
    glEnd();
    glFlush();

}

// y : -9-9 ,,,, x : 0-8
void drawChairs()
{

    int peopleCount = 0, jTemp = 0, iTemp = -9;

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = 8; i > -1; i--)
    {
        iTemp = i;
        int f = 0;
        for (int j = -9; j < 8; j++)
        {
            jTemp = j;
            glVertex2i(i, j);
            peopleCount++;
            if (peopleCount == customer_count)
            {
                f = 1;
                break;
            }
        }
        if (f)
            break;
    }
    glEnd();



    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i > -1; i--)
    {

        for (int j = jTemp + 1; j < 8; j++)
        {
            glVertex2i(i, j);
        }
        jTemp = -10;
    }
    glEnd();
    glFlush();




}
void drawCashier1()
{
    int peopleCount = 0, jTemp = -9, iTemp = 6;
    int xShift = -9; // Shift in the x-direction
    int yShift = 8;   // Shift in the y-direction
    int ySpace = 1;   // Space between the two rows

    glPointSize(5);

    // Draw all points in blue first
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i >= 0; i--)
    {
        for (int j = jTemp; j < jTemp + 1; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
        }
    }
    glEnd();

    // Overwrite with red points based on the number of people
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = iTemp; i >= 0 && peopleCount < c.numberOfEmployeesPerTeam; i--)
    {
        for (int j = jTemp; j < jTemp + 1 && peopleCount < c.numberOfEmployeesPerTeam; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace && peopleCount < c.numberOfEmployeesPerTeam; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }
    }
    glEnd();
    glFlush();
}

void drawCashier2()
{
    int peopleCount = 0, jTemp = -9, iTemp = 6;
    int xShift = -9; // Shift in the x-direction
    int yShift = 17;   // Shift in the y-direction
    int ySpace = 1;   // Space between the two rows

    glPointSize(5);

    // Draw all points in blue first
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i >= 0; i--)
    {
        for (int j = jTemp; j < jTemp + 1; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
        }
    }
    glEnd();

    // Overwrite with red points based on the number of people
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = iTemp; i >= 0 && peopleCount < c.numberOfEmployeesPerTeam; i--)
    {
        for (int j = jTemp; j < jTemp + 1 && peopleCount < c.numberOfEmployeesPerTeam; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace && peopleCount < c.numberOfEmployeesPerTeam; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }
    }
    glEnd();
    glFlush();
}


void drawCashier3()
{
    int peopleCount = 0, jTemp = -9, iTemp = 6;
    int xShift = -9; // Shift in the x-direction
    int yShift = 0;   // Shift in the y-direction
    int ySpace = 1;   // Space between the two rows

    glPointSize(5);

    // Draw all points in blue first
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i >= 0; i--)
    {
        for (int j = jTemp; j < jTemp + 1; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
        }
    }
    glEnd();

    // Overwrite with red points based on the number of people
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = iTemp; i >= 0 && peopleCount < c.numberOfEmployeesPerTeam; i--)
    {
        for (int j = jTemp; j < jTemp + 1 && peopleCount < c.numberOfEmployeesPerTeam; j++) // First row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }

        for (int j = jTemp + 1 + ySpace; j < jTemp + 2 + ySpace && peopleCount < c.numberOfEmployeesPerTeam; j++) // Second row
        {
            glVertex2i(i + xShift, j + yShift);
            peopleCount++;
        }
    }
    glEnd();
    glFlush();
}




void drawTellers()
{
    glBegin(GL_POLYGON_BIT);
    glColor3f(7, 0.3, 10);
    glVertex2i(20, 12);
    glVertex2i(12, 12);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0, 0, 255);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.3, 0.8, 0.2);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.356, 0.514, 0.999);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glVertex2i(20, -12);
    glVertex2i(12, -12);
    glEnd();
}

void drawText()
{
    glColor3f(1.0, 0.8, 0.5);
    char
            customerCount[30] = "customerCount : ",
            team1[30] = "Team 1  Employees Count :  ",
            team2[30]="Team 2  Employees Count :  ",
            team3[30] = "Team 3 Employees Count : ",
            Milk_on_storage[30] = "Milk : ",
            bakery[30] = " Eggs : ",
            dair_eggs[30] = "Bread  : ",
            packaged[30] = "Apples : ",
            employees1[5] = "",
            employees3[5] = "",
            employees2[5]="",
            number_of_customers[5] = "",
            milk_on_storage[7]="",
            bread_on_storage[8]="",
            eggs_on_storage[8]="",
            apples_on_storage[8]="";




    char milk_in_shelf_char[20], eggs_in_shelf_char[20], bread_in_shelf_char[20], apples_in_shelf_char[20];


    sprintf(number_of_customers, "%d", customer_count);

    // Prepare strings for queue counts
    sprintf(milk_in_shelf_char, "Milk in shelf : %d", milk_in_shelf);
    sprintf(eggs_in_shelf_char, "Bread in shelf : %d", eggs_in_shelf );
    sprintf(bread_in_shelf_char, "Eggs in shelf : %d", bread_in_shelf);
    sprintf(apples_in_shelf_char, "Apples in shelf  %d", apples_in_shelf);

    sprintf(milk_on_storage, "%d", milk_count);
    sprintf(bread_on_storage, "%d", bread_count );
    sprintf(eggs_on_storage, "%d", eggs_count);
    sprintf(apples_on_storage, "%d", apples_count);

    sprintf(employees1, "%d", c.numberOfEmployeesPerTeam);
    sprintf(employees2, "%d", c.numberOfEmployeesPerTeam);
    sprintf(employees3, "%d", c.numberOfEmployeesPerTeam);


    strcat(team1, employees1);
    strcat(team2,employees2);
    strcat(team3, employees3);

    strcat(customerCount, number_of_customers);
    strcat(Milk_on_storage, milk_on_storage);
    strcat(bakery, bread_on_storage);
    strcat(dair_eggs, eggs_on_storage);
    strcat(packaged, apples_on_storage);





    renderText(-9, 12, GLUT_BITMAP_TIMES_ROMAN_24, team1); // first cashier queue
    renderText(-9, 3, GLUT_BITMAP_TIMES_ROMAN_24, team2); // cashier2 total sales
    renderText(-9, -15, GLUT_BITMAP_TIMES_ROMAN_24, team3); // cashier3 queue
    renderText(2, 10, GLUT_BITMAP_TIMES_ROMAN_24, customerCount); //cousomer count
    //  renderText(-18, -2, GLUT_BITMAP_TIMES_ROMAN_24, cashierCount); // cahier count
    renderText(14, 9, GLUT_BITMAP_TIMES_ROMAN_24, Milk_on_storage);
    renderText(14, 3, GLUT_BITMAP_TIMES_ROMAN_24, bakery);
    renderText(14, -3, GLUT_BITMAP_TIMES_ROMAN_24, dair_eggs);
    renderText(14, -9, GLUT_BITMAP_TIMES_ROMAN_24, packaged);
    renderText(13, 18, GLUT_BITMAP_TIMES_ROMAN_24, drawer_current_time);


    renderText(-18, 8, GLUT_BITMAP_TIMES_ROMAN_24, milk_in_shelf_char); // Queue info for Cashier 1
    renderText(-18, 6, GLUT_BITMAP_TIMES_ROMAN_24, bread_in_shelf_char); // Queue info for Cashier 2
    renderText(-18, 4, GLUT_BITMAP_TIMES_ROMAN_24,  eggs_in_shelf_char); // Queue info for Cashier 3
    renderText(-18, 2, GLUT_BITMAP_TIMES_ROMAN_24, apples_in_shelf_char);  // Item info for Cashier 1


    renderText(13, 18, GLUT_BITMAP_TIMES_ROMAN_24, drawer_current_time);

}

void renderText(int x, int y, void *font, char *string)
{
    glRasterPos2f(x, y);
    int len, i;
    len = strlen(string);
    for (i = 0; i < len && string[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}




// read data from shared memory in this function
void readDataFromSharedMemory()
{



    customer_count = get_total_customers();

    get_shared_products(items_count);

    milk_count = items_count[0].storageAmount;
    eggs_count = items_count[1].storageAmount;
    bread_count = items_count[2].storageAmount;
    apples_count = items_count[3].storageAmount;

    milk_in_shelf = items_count[0].shelfAmount;
    eggs_in_shelf = items_count[1].shelfAmount;
    bread_in_shelf = items_count[2].shelfAmount;
    apples_in_shelf = items_count[3].shelfAmount;

    drawer_current_time = time_to_string();





}
