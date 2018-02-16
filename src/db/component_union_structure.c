#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "config.h"
#include "graph.h"


typedef struct ver{
	vertexid_t id;
	int flag;
	struct ver *next;
} st_ver;


typedef struct edg{
	vertexid_t id1,id2;
	int flag;
	struct edg *next;
} st_edg;


static int
is_edge_present(edge_t key, st_edg *head)
{
	st_edg *temp = head;
	while(temp != NULL){
		if(temp->id1 == key->id1 && temp->id2 == key->id2){
			temp->flag = 1;
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

static int
is_vertex_present(vertex_t key, st_ver *head)
{
	st_ver *temp = head;
	while(temp != NULL){
		if(temp->id == key->id){
			temp->flag = 1;
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

static component_t
component_union_structure(component_t c1, component_t c2)
{
	
	struct vertex new_comp_ver;
	struct edge new_comp_edg;
	int readlen;
	off_t off;
	char *buf;
	st_ver *v_head = NULL,*v_tmp,*v_fwd;
	st_edg *e_head = NULL,*e_tmp,*e_fwd;
	int new_v_fd,new_e_fd;
	int c1_size_v,c2_size_v,c1_size_e,c2_size_e;
	struct vertex c1_temp,c1_v_dum,c2_v_dum;
	struct edge c1_e_temp,c1_e_dum,c2_e_dum;
	ssize_t len;
	char s[BUFSIZE];

	vertex_init(&c1_temp);
	vertex_init(&c1_v_dum);
	vertex_init(&c2_v_dum);
	vertex_init(&new_comp_ver);

	edge_init(&new_comp_edg);
	edge_init(&c1_e_temp);
	edge_init(&c1_e_dum);
	edge_init(&c2_e_dum);

	tuple_init(&c1_v_dum.tuple, c1->sv);
	tuple_init(&c1_e_dum.tuple, c1->se);
	tuple_init(&c2_v_dum.tuple, c2->sv);
	tuple_init(&c2_e_dum.tuple, c2->se);

	c1_size_v = schema_size(c1->sv);
	c2_size_v = schema_size(c2->sv);
	c1_size_e = schema_size(c1->se);
	c2_size_e = schema_size(c2->se);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/123/v",grdbdir, gno);
	new_v_fd = open(s, O_WRONLY | O_CREAT, 0644);

	// Start of vertex loop for component 1
	readlen = sizeof(vertexid_t) + c1_size_v;
	buf = malloc(readlen);

	for (off = 0;;) {
		v_tmp = (st_ver*)malloc(sizeof(st_ver));
		lseek(c1->vfd, off, SEEK_SET);
		len = read(c1->vfd, buf, sizeof(vertexid_t));
		if (len != sizeof(vertexid_t)) {
			break;
		}
		off += sizeof(vertexid_t);
		v_tmp->id = *((vertexid_t *) buf);
		v_tmp->next = NULL;
		v_tmp->flag = 0;
		if(v_head == NULL){
			v_head = v_tmp;
			v_fwd = v_head;
		}
		else{
			v_fwd->next = v_tmp;
			v_fwd = v_fwd->next;
		}
		off += c1_size_v;
	}
	// End of vertex loop for component 1


	// Start of edge loop for component 1
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/123/e",grdbdir, gno);
	new_e_fd = open(s, O_WRONLY | O_CREAT, 0644);

for (off = 0;; off += (sizeof(vertexid_t) << 1) + c1_size_e) {
	e_tmp = (st_edg*)malloc(sizeof(st_edg));
		lseek(c1->efd, off, SEEK_SET);
		len = read(c1->efd, buf, sizeof(vertexid_t) << 1);
		if (len != sizeof(vertexid_t) << 1) {
			break;
		}
		e_tmp->id1 = *((vertexid_t *) buf);
		e_tmp->id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));

		e_tmp->next = NULL;
		e_tmp->flag = 0;
		if(e_head == NULL){
			e_head = e_tmp;
			e_fwd = e_head;
		}
		else{
			e_fwd->next = e_tmp;	
			e_fwd = e_fwd->next;
		}
	}
	// End of edge loop for component 1


	// Start of vertex loop for component 2
	readlen = sizeof(vertexid_t) + c2_size_v;
	buf = malloc(readlen);
	for (off = 0;;) {
		lseek(c2->vfd, off, SEEK_SET);
		len = read(c2->vfd, buf, sizeof(vertexid_t));
		if (len != sizeof(vertexid_t)) {
			break;
		}
		off += sizeof(vertexid_t);
		c1_temp.id = *((vertexid_t *) buf);
			
		tuple_init(&(c1_temp.tuple), c2->sv);
		len = vertex_read(&c1_temp, c2->sv, c2->vfd);
		off += c2_size_v;

		if(is_vertex_present(&c1_temp,v_head)){

			new_comp_ver.id = c1_temp.id;
			tuple_init(&(new_comp_ver.tuple), c1->sv);
			len = vertex_read(&new_comp_ver, c1->sv, c1->vfd);
			len = write(new_v_fd, &(c1_temp.id), sizeof(vertexid_t));
			len = write(new_v_fd, new_comp_ver.tuple->buf, c1_size_v);
			len = write(new_v_fd, c1_temp.tuple->buf, c2_size_v);
		}
		else{
			len = write(new_v_fd, &(c1_temp.id), sizeof(vertexid_t));
			len = write(new_v_fd, c1_v_dum.tuple->buf, c1_size_v);
			len = write(new_v_fd, c1_temp.tuple->buf, c2_size_v);
		}
		
	}
	// End of vertex loop for component 2


	// Start of edge loop for component 2
	free(buf);
	readlen = sizeof(vertexid_t) + sizeof(vertexid_t) + c2_size_e;
	buf = malloc(readlen);
	for (off = 0;; off += (sizeof(vertexid_t) << 1) + c2_size_e) {
		lseek(c2->efd, off, SEEK_SET);
		len = read(c2->efd, buf, sizeof(vertexid_t) << 1);
		if (len != sizeof(vertexid_t) << 1) {
			break;
		}
		c1_e_temp.id1 = *((vertexid_t *) buf);
		c1_e_temp.id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));

		tuple_init(&(c1_e_temp.tuple), c2->se);
		len = edge_read(&c1_e_temp, c2->se, c2->efd);
		if(is_edge_present(&c1_e_temp,e_head)){
			new_comp_edg.id1 = c1_e_temp.id1;
			new_comp_edg.id2 = c1_e_temp.id2;
			tuple_init(&(new_comp_edg.tuple), c1->se);
			len = edge_read(&new_comp_edg, c1->se, c1->efd);
			len = write(new_e_fd, &(c1_e_temp.id1), sizeof(vertexid_t));
			len = write(new_e_fd, &(c1_e_temp.id2), sizeof(vertexid_t));
			len = write(new_e_fd, new_comp_edg.tuple->buf, c1_size_e);
			len = write(new_e_fd, c1_e_temp.tuple->buf, c2_size_e);
		}
		else{
			len = write(new_e_fd, &(c1_e_temp.id1), sizeof(vertexid_t));
			len = write(new_e_fd, &(c1_e_temp.id2), sizeof(vertexid_t));
			len = write(new_e_fd, c1_e_dum.tuple->buf, c1_size_e);
			len = write(new_e_fd, c1_e_temp.tuple->buf, c2_size_e);
		}
	}
	// End of edge loop for component 2



	// Vertex leftovers
v_fwd = v_head;
while(v_fwd != NULL){
	if(v_fwd->flag == 0){
		new_comp_ver.id = v_fwd->id;
		tuple_init(&(new_comp_ver.tuple), c1->sv);
		new_comp_ver.tuple->s = schema_read(c1->vfd, c1->el);
		len = vertex_read(&new_comp_ver, c1->sv, c1->vfd);
		len = write(new_v_fd, &(new_comp_ver.id), sizeof(vertexid_t));
		len = write(new_v_fd, new_comp_ver.tuple->buf, c1_size_v);
		len = write(new_v_fd, c2_v_dum.tuple->buf, c2_size_v);
	}
	v_fwd = v_fwd->next;
}
	// End of vertex leftovers

	// Edge letfovers
e_fwd = e_head;
while(e_fwd != NULL){
	if(e_fwd->flag == 0){
		new_comp_edg.id1 = e_fwd->id1;
		new_comp_edg.id2 = e_fwd->id2;
		tuple_init(&(new_comp_edg.tuple), c1->sv);
		new_comp_edg.tuple->s = schema_read(c1->vfd, c1->el);
		len = edge_read(&new_comp_edg, c1->se, c1->efd);
		len = write(new_e_fd, &(new_comp_edg.id1), sizeof(vertexid_t));
		len = write(new_e_fd, &(new_comp_edg.id2), sizeof(vertexid_t));
		len = write(new_e_fd, new_comp_edg.tuple->buf, c1_size_v);
		len = write(new_e_fd, c2_e_dum.tuple->buf, c2_size_v);
	}
	e_fwd = e_fwd->next;
}
	//End of edge leftovers
	return NULL;
}
