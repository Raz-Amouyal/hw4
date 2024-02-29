#include "grades.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "linked-list.h"


#define FAIL 1
#define SUCCESS 0
#define ERROR -1
#define MIN_GRADE 0
#define MAX_GRADE 100


struct grades {
    /* Linked-list of the students */
    struct list *students;

};
/* The students linked-list will have these as elements */
struct student {
    int id;
    char *name;
    struct list *courses; /*Linked-list of the student's courses*/

};
/* The courses linked-list will have these as elements */
struct course {
    char *name;
    int grade;
};

/* User-functions for the linked-list */
int student_clone(void *elem, void **out){
    struct student *student;
    struct student *clone;

    student=(struct student*)elem;

    clone=(struct student*)malloc(sizeof(*clone));
    if (!clone){
        return FAIL;
    }
    clone->name = (char*)malloc(sizeof(char)*(strlen(student->name)+1));
    if(!clone->name){
        free(clone);
        return FAIL;
    }
    strcpy(clone->name, student->name);
    clone->id = student->id;
    clone->courses = student->courses;
    *out = clone;
    return SUCCESS;
}
void student_destroy(void *elem){
    if(!elem){
        return;
    }

    struct student *student;
    student = (struct student*)elem;

    free(student->name);
    list_destroy(student->courses);
    free(student);
}
int course_clone(void *elem, void **out){
    struct course *course;
    struct course *clone;


    course = (struct course*)elem;

    clone=(struct course*)malloc(sizeof(*clone));
    if (!clone){
        return FAIL;
    }
    clone->name = (char*)malloc(sizeof(char)*(strlen(course->name)+1));
    if(!clone->name){
        free(clone);
        return FAIL;
    }
    strcpy(clone->name, course->name);
    clone->grade = course->grade;
    *out = clone;
    return SUCCESS;
}
void course_destroy(void *elem){
    if(!elem){
        return;
    }

    struct course *course;
    course = (struct course*)elem;

    free(course->name);
    free(course);
}

/* Self use functions */
/* check existence by id */
bool student_exist(struct list *list, int id){
    struct iterator *i;
    struct student *student;
    for (i= list_begin(list); i ; i= list_next(i)) {
        student = list_get(i);
        if(student->id == id){
            return true;
        }
    }
    return false;
}
/* check existence by name */
bool course_exist(struct list *list, const char *name){
    struct iterator *i;
    struct course *course;
    for (i= list_begin(list); i ; i= list_next(i)) {
        course = list_get(i);
        if(course->name == name){
            return true;
        }
    }
    return false;
}
/* Return pointer to the student with "id" on success, NULL if fails */
struct student *student_get(struct list *list, int id){
    struct iterator *i;
    struct student *student;
    for (i= list_begin(list); i ; i= list_next(i)) {
        student = list_get(i);
        if(student->id == id){
            return student;
        }
    }
    return NULL;
}



struct grades* grades_init(){
    struct grades *grades;
    struct list *students;

    grades = (struct grades*)malloc(sizeof(*grades));
    students = list_init(student_clone, student_destroy);
    if(!grades || !students){
        free(grades);
        free(students);
        return NULL;
    }
    grades->students = students;
    return grades;
}

void grades_destroy(struct grades *grades){
    list_destroy(grades->students);
    free(grades);
}

/* Add new student to the tail of the list - first to added first in list */
int grades_add_student(struct grades *grades, const char *name, int id){
    if(!grades || student_exist(grades->students, id)){
        return FAIL;
    }

    struct student student;
    student.name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    if(!student.name){
        return FAIL;
    }
    strcpy(student.name, name);
    student.id = id;
    student.courses= list_init(course_clone, course_destroy);

    if(!student.courses){
        free(student.name);
        return FAIL;
    }

    /* Insert the student and check if succeeded - func return 0 on success */
    if(list_push_back(grades->students, &student)){
        free(student.name);
        list_destroy(student.courses);
        return FAIL;
    }
    free(student.name);
    return SUCCESS;
}

/* Add new grade to the tail of the list - first to added first in list */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {
    if (!grades || grade < MIN_GRADE || grade > MAX_GRADE) {
        return FAIL;
    }

    struct student *student;

    student = student_get(grades->students, id);

    if (!student || course_exist(student->courses, name)) {
        return FAIL;
    }

    struct course course;
    course.name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (!course.name) {
        return FAIL;
    }
    strcpy(course.name, name);
    course.grade = grade;

    /* Insert the course and check if succeeded - func return 0 on success */
    int res = list_push_back(student->courses, &course);
    free(course.name);
    return res;
}




float grades_calc_avg(struct grades *grades, int id, char **out){
    if(!grades){
        *out = NULL;
        return ERROR;
    }

    struct student *student;
    struct course *course;
    struct iterator *i;
    int total=0;
    int count=0;
    float avg;

    student = student_get(grades->students, id);
    if (!student){
        *out = NULL;
        return ERROR;
    }

    *out = (char*)malloc(sizeof(char)*(strlen(student->name)+1));
    if(!*out){
        *out = NULL;
        return ERROR;
    }

    strcpy(*out, student->name);

    for (i= list_begin(student->courses); i ; i= list_next(i)) {
        course = list_get(i);
        total += course->grade;
        count++;
    }
    avg = (count>0) ? (double)total/count : 0;
    return avg;
}

int grades_print_student(struct grades *grades, int id){
    if(!grades){
        return FAIL;
    }

    struct student *student;
    struct course *course;
    struct iterator *i;


    student = student_get(grades->students, id);
    if (!student){
        return FAIL;
    }
    printf("%s %d:",student->name, student->id);


    i= list_begin(student->courses);
    while (i){
        course = list_get(i);
        printf(" %s %d", course->name, course->grade);
        i= list_next(i);
        if(i){
            printf(",");
        } else{

        }
    }
    printf("\n");


    /*
    for (i= list_begin(student->courses); i ; i= list_next(i)) {
        course = list_get(i);
        printf(" %s %d,", course->name, course->grade);
    }
    printf("\n");
     */
    return SUCCESS;
}

int grades_print_all(struct grades *grades){
    if(!grades){
        return FAIL;
    }

    struct iterator *i;
    struct iterator *j;
    struct student *student;
    struct course *course;
    for (i= list_begin(grades->students); i ; i= list_next(i)) {
        student = list_get(i);
        printf("%s %d:",student->name, student->id);

        j= list_begin(student->courses);
        while (j){
            course = list_get(j);
            printf(" %s %d", course->name, course->grade);
            j= list_next(j);
            if(j){
                printf(",");
            } else{

            }
        }
        printf("\n");


        /*
        for (j= list_begin(student->courses); j ; j= list_next(j)) {
            course = list_get(j);
            printf(" %s %d,", course->name, course->grade);
        }
        printf("\n");
        */
    }
    return SUCCESS;
}


