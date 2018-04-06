#include <assert.h>
#include <unistd.h>
#include "graph.h"

static void
vertex_copy_values(char *old, schema_t sold, char *new, schema_t snew)
{
	attribute_t attrold, attrnew;
	struct tuple told, tnew;
	int offold, offnew;
	base_types_t bt;

	for (attrold = sold->attrlist;
	     attrold != NULL;
	     attrold = attrold->next) {
		attrnew = schema_find_attr_by_name(snew, attrold->name);
		if (attrnew == NULL)
			continue;

		/* Copy values from one tuple to the other */
		memset(&told, 0, sizeof(struct tuple));
		told.s = sold;
		told.len = schema_size(sold);
		told.buf = old;

		memset(&tnew, 0, sizeof(struct tuple));
		tnew.s = snew;
		tnew.len = schema_size(snew);
		tnew.buf = new;

		offold = tuple_get_offset(&told, attrold->name);
		offnew = tuple_get_offset(&tnew, attrold->name);

		bt = schema_find_type_by_name(sold, attrold->name);
		if (bt == BASE_TYPES_MAX)
			continue;
		switch (bt) {
		case CHARACTER:
			tuple_set_char(new + offnew,
				tuple_get_char(old + offold));
			break;

		case VARCHAR:
//			tuple_set_varchar(new + offnew,
//				tuple_get_varchar(old + offold));
			break;

		case BOOLEAN:
			tuple_set_bool(new + offnew,
				tuple_get_bool(old + offold));
			break;

		case INTEGER:
			tuple_set_int(new + offnew,
				tuple_get_int(old + offold));
			break;

		case FLOAT:
			tuple_set_float(new + offnew,
				tuple_get_float(old + offold));
			break;

		case DOUBLE:
			tuple_set_double(new + offnew,
				tuple_get_double(old + offold));
			break;

		case DATE:
//			tuple_set_date(new + offnew,
//				tuple_get_date(old + offold));
			break;

		case TIME:
//			tuple_set_time(new + offnew,
//				tuple_get_time(old + offold));
			break;

		case ENUM:
			/* Handled elsewhere */
		case BASE_TYPES_MAX:
			break;
		}
	}
}

void
vertex_write_from(
	vertexid_t id,
	char *buf,
	schema_t schema,
	int fdnew,
	schema_t schemanew)
{
	char *bufnew;
	int len, off, size, sizenew;

	/*
	 * Given a tuple buffer and its schema, copy the values
	 * into a new tuple and write the new tuple to disk
	 */
	assert (buf != NULL);
	assert (schema != NULL);
	
	size = schema_size(schema);
	sizenew = schema_size(schemanew);

	/* Search for the proper tuple in the new file */
	bufnew = malloc(sizenew);
	assert (bufnew != NULL);
	for (off = 0;; off += sizeof(vertexid_t) + size) {
		lseek(fdnew, off, SEEK_SET);

		/* Read vertex id and tuple buf */
		memset(bufnew, 0, sizenew);
		len = read(fdnew, bufnew, sizeof(vertexid_t) + sizenew);
		if (len == 0) {
			break;
		}
		if (*((vertexid_t *) bufnew) == id) {
			/* Fill in the values of the new tuple buffer */
			vertex_copy_values(
				char *old,
				schema_t sold,
				char *new,
				schema_t snew)

			/* Write the new tuple back out */
			lseek(fdnew, off, SEEK_SET);
			write(fdnew, bufnew, sizeof(vertexid_t) + sizenew);
			free(bufnew);
			return;
		}
	}
	free(bufnew);

	/*
	 * The vertex id was not found so "drop the head" and write the
	 * new vertex id and tuple
	 */

}
