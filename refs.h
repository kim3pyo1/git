#ifndef REFS_H
#define REFS_H

struct ref_lock {
	char *ref_name;
	char *orig_ref_name;
	struct lock_file *lk;
	unsigned char old_sha1[20];
	int lock_fd;
	int force_write;
};

#define REF_ISSYMREF 01
#define REF_ISPACKED 02

/*
 * Calls the specified function for each ref file until it returns nonzero,
 * and returns the value
 */
typedef int each_ref_fn(const char *refname, const unsigned char *sha1, int flags, void *cb_data);
extern int head_ref(each_ref_fn, void *);
extern int for_each_ref(each_ref_fn, void *);
extern int for_each_tag_ref(each_ref_fn, void *);
extern int for_each_branch_ref(each_ref_fn, void *);
extern int for_each_remote_ref(each_ref_fn, void *);

extern int peel_ref(const char *, unsigned char *);

/** Locks a "refs/" ref returning the lock on success and NULL on failure. **/
extern struct ref_lock *lock_ref_sha1(const char *ref, const unsigned char *old_sha1);

/** Locks any ref (for 'HEAD' type refs). */
#define REF_NODEREF	0x01
extern struct ref_lock *lock_any_ref_for_update(const char *ref, const unsigned char *old_sha1, int flags);

/** Close the file descriptor owned by a lock and return the status */
extern int close_ref(struct ref_lock *lock);

/** Close and commit the ref locked by the lock */
extern int commit_ref(struct ref_lock *lock);

/** Release any lock taken but not written. **/
extern void unlock_ref(struct ref_lock *lock);

/** Writes sha1 into the ref specified by the lock. **/
extern int write_ref_sha1(struct ref_lock *lock, const unsigned char *sha1, const char *msg);

/** Reads log for the value of ref during at_time. **/
extern int read_ref_at(const char *ref, unsigned long at_time, int cnt, unsigned char *sha1, char **msg, unsigned long *cutoff_time, int *cutoff_tz, int *cutoff_cnt);

/* iterate over reflog entries */
typedef int each_reflog_ent_fn(unsigned char *osha1, unsigned char *nsha1, const char *, unsigned long, int, const char *, void *);
int for_each_reflog_ent(const char *ref, each_reflog_ent_fn fn, void *cb_data);

/*
 * Calls the specified function for each reflog file until it returns nonzero,
 * and returns the value
 */
extern int for_each_reflog(each_ref_fn, void *);

#define CHECK_REF_FORMAT_OK 0
#define CHECK_REF_FORMAT_ERROR (-1)
#define CHECK_REF_FORMAT_ONELEVEL (-2)
#define CHECK_REF_FORMAT_WILDCARD (-3)
extern int check_ref_format(const char *target);

/** rename ref, return 0 on success **/
extern int rename_ref(const char *oldref, const char *newref, const char *logmsg);

/** resolve ref in nested "gitlink" repository */
extern int resolve_gitlink_ref(const char *name, const char *refname, unsigned char *result);

/** lock a ref and then write its file */
enum action_on_err { MSG_ON_ERR, DIE_ON_ERR, QUIET_ON_ERR };
int update_ref(const char *action, const char *refname,
		const unsigned char *sha1, const unsigned char *oldval,
		int flags, enum action_on_err onerr);

#endif /* REFS_H */
