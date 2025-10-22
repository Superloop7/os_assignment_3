/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	int left = leftstart;
	int right = rightstart;
	int index = leftstart;

	while (left <= leftend && right <= rightend) {
		if (A[left] <= A[right]) {
			B[index] = A[left];
			left ++;
		} else {
			B[index] = A[right];
			right ++;
		}
		index ++;
	}
	
	/* copy remaining elements */
	while (left <= leftend) {
		B[index] = A[left];
		left ++;
		index ++;
	}
	while (right <= rightend) {
		B[index] = A[right];
		right ++;
		index ++;
	}

	/* copy back to A */
	memcpy(&A[leftstart], &B[leftstart], sizeof(int) * (rightend - leftstart + 1));
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	if (left >= right) return;

	int middle = (left + right) / 2;
	my_mergesort(left, middle);
	my_mergesort(middle + 1, right);
	merge(left, middle, middle + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
	/* Preventing the null pointers */
	if (arg == NULL) return NULL;

	struct argument *a = (struct argument *) arg;
	int left = a -> left;
	int right = a -> right;
	int level = a -> level;

	/* only one element in array */
	if (left >= right) {
		return NULL;
	}
	
	/* if reach the cutoff, the thread amount is meet the request, do all mergesort */
	if (level >= cutoff) {
		my_mergesort(left, right);
		return NULL;
	}

	int mid = (left + right) / 2;

	pthread_t t1, t2;
	struct argument *la = buildArgs(left, mid, level + 1);
	struct argument *ra = buildArgs(mid+1, right, level + 1);
	
	/* create two threads to sort two halves */
	pthread_create(&t1, NULL, parallel_mergesort, (void *)la);
	pthread_create(&t2, NULL, parallel_mergesort, (void *)ra);

	/* wait for both thread finish */
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	/* free the child argument */
	free(la);
	free(ra);

	/* merge the two sorted halves */
	merge(left, mid, mid + 1, right);

	return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	struct argument *a = (struct argument *)malloc(sizeof(struct argument));
	if (a == NULL) return NULL;
	a->left = left;
	a->right = right;
	a->level = level;
	return a;
}

