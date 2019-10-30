#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>

#include "./include/tcp.h"
#include "./include/camera.h"
#include "./include/jpg.h"
#include "./include/convert.h"

#define CAMERA_DEV_PATH	"/dev/video0"

struct jpg_buf_t *jpg;
pthread_t cam_tid;
pthread_mutex_t jpg_mutex;

void *thread_cam(void *arg);
void *thread_srv(void *arg);
extern int client_process(int connfd, struct jpg_buf_t *jpg);

int main(int argc, char *argv[])
{
	int ret;

	pthread_mutex_init(&jpg_mutex, NULL);

	ret = pthread_create(&cam_tid, NULL, thread_cam, NULL);
	if (ret) {
		errno = ret;
		perror("create camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("create camera thread success\n");

	ret = pthread_detach(cam_tid);
	if (ret) {
		errno = ret;
		perror("detach camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach camera thread success\n");

	thread_srv(NULL);

	exit(EXIT_SUCCESS);
}

void *thread_cam(void *arg)
{
	int i;
	int fd;
	int ret;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int index;
	unsigned int ismjpeg;
	char *yuv;
	char *rgb;

	/* A8的屏幕比较小，所以设了较低的像素 */
	width = 1024;
	height = 1024;
	fd = camera_init(CAMERA_DEV_PATH, &width, &height, &size, &ismjpeg);
	if (fd == -1)
		exit(EXIT_FAILURE);
	printf("width: %d\n", width);
	printf("height: %d\n", height);

	ret = camera_start(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);

	jpg = malloc(sizeof(struct jpg_buf_t));
	if (!jpg) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	if (!ismjpeg) {
		rgb = malloc(width * height * 3);
		convert_rgb_to_jpg_init();
	}

	/* 采集几张图片丢弃 */
	for (i = 0; i < 8; i++) {
		ret = camera_dqbuf(fd, (void **)&yuv, &size, &index);
		if (ret == -1)
			exit(EXIT_FAILURE);

		ret = camera_eqbuf(fd, index);
		if (ret == -1)
			exit(EXIT_FAILURE);
	}

	fprintf(stdout, "init camera success\n");


	/* 循环采集图片 */
	while (1) {
		ret = camera_dqbuf(fd, (void **)&yuv, &size, &index);
		if (ret == -1)
			exit(EXIT_FAILURE);

		/*if (ismjpeg) {
			pthread_mutex_lock(&jpg_mutex);
			memcpy(jpg->buf, yuv, size);
			jpg->len = size;
			pthread_mutex_unlock(&jpg_mutex);
		} else {
			convert_yuv_to_rgb(yuv, rgb, width, height, 24);
			pthread_mutex_lock(&jpg_mutex);
			jpg->len = convert_rgb_to_jpg_work(rgb, jpg->buf, width, height, 24, 80);
			pthread_mutex_unlock(&jpg_mutex);
		}*/
		
        convert_yuv_to_rgb(yuv, rgb, width, height, 24);
			pthread_mutex_lock(&jpg_mutex);
			jpg->len = convert_rgb_to_jpg_work(rgb, jpg->buf, width, height, 24, 80);
			pthread_mutex_unlock(&jpg_mutex);
			
		ret = camera_eqbuf(fd, index);
		if (ret == -1)
			exit(EXIT_FAILURE);
	}

	/* 代码不应该运行到这里 */
	if (!ismjpeg) {
		convert_rgb_to_jpg_exit();
		free(rgb);
	}
	free(jpg);

	ret = camera_stop(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);

	ret = camera_exit(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);
}

void *thread_srv(void *arg)
{
	int listenfd;
	int connfd;

	listenfd = tcp_server_init(NULL, "8888");
	if (listenfd == -1)
		exit(EXIT_FAILURE);
	else
		fprintf(stdout, "init server success\n");

	while (1) {
		if ((connfd = tcp_server_wait_connect(listenfd)) != -1) {
			while (1) {
				if (client_process(connfd, jpg) == -1) {
					tcp_server_disconnect(connfd);
					break;
				}
			}
		}
	}
}
