#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct sstf_data {
        struct list_head queue;
        sector_t pos;
        int left_right;
};

/* Taken from noop scheduler */
static void sstf_merged_requests(struct request_queue *q, struct request *rq, struct request *next)
{
	list_del_init(&next->queuelist);
}

/* This is the function that I have modified */
static int sstf_dispatch(struct request_queue *q, int force)
{
	struct sstf_data *nd = q->elevator->elevator_data;

        sector_t cur_req;

	if (!list_empty(&nd->queue)) {
                struct request *rq;
		struct request *prev;
		struct request *next;

		prev = list_entry(nd->queue.prev, struct request, queuelist);
		next = list_entry(nd->queue.next, struct request, queuelist);
                rq = next;

                /* If next was equal to prev, there would only be one request */
                if(next != prev){
                        printk("SSTF: There is more than one request to be dispatched\n");

                        /* If we are going forward */
                        if(nd->left_right == 1){
                                cur_req = blk_rq_pos(next);
                                /* If there is a shorter request in the forward direction, assign it to rq*/
                                if(cur_req > nd->pos){
                                        printk("SSTF: Shorter request in forward direction\n");
                                        rq = next;
                                }
                                /* if there is not a shorter request, switch directions */
                                else if(cur_req < nd->pos){
                                        printk("SSTF: Shorter request in backward direction\n");
                                        nd->left_right = 0;
                                        rq = prev;
                                }
                        }
                        else if(nd->left_right == 0){
                                cur_req = blk_rq_pos(prev);
                                /* If there is a shorter request in the backwards direction assign it to rq */
                                if(cur_req < nd->pos){
                                        printk("SSTF: Shorter request in forward direction\n");
                                        rq = prev;
                                }
                                /* If there is not a shorter request, switch directions */
                                else if(cur_req > nd->pos){
                                        printk("SSTF: Shorter request in backward direction\n");
                                        nd->left_right = 1;
                                        rq = next;
                                }
                        }
                }
                /* There is only one request here */
                else{
                        rq = next;
                        printk("SSTF: There was only one request to be dispatched\n");
                }

                nd->pos = blk_rq_pos(rq) + blk_rq_sectors(rq);
                list_del_init(&rq->queuelist);
                elv_dispatch_sort(q, rq);
                return 1;
        }
        printk("SSTF: The list is currently empty\n");
        return 0;
}

static void sstf_add_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	list_add_tail(&rq->queuelist, &nd->queue);
}

/* Taken from noop scheduler */
static struct request *
sstf_former_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

/* Taken from noop scheduler */
static struct request *
sstf_latter_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

/* Taken from noop scheduler */
static int sstf_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct sstf_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->queue);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

/* Taken from noop scheduler */
static void sstf_exit_queue(struct elevator_queue *e)
{
        struct sstf_data *nd = e->elevator_data;

        BUG_ON(!list_empty(&nd->queue));
        kfree(nd);
}

static struct elevator_type elevator_sstf = {
        .ops = {
                .elevator_merge_req_fn      = sstf_merged_requests,
                .elevator_dispatch_fn       = sstf_dispatch,
                .elevator_add_req_fn        = sstf_add_request,
                .elevator_former_req_fn     = sstf_former_request,
                .elevator_latter_req_fn     = sstf_latter_request,
                .elevator_init_fn       = sstf_init_queue,
                .elevator_exit_fn       = sstf_exit_queue,
        },
        .elevator_name = "sstf",
        .elevator_owner = THIS_MODULE,
};

/* Taken from noop scheduler */
static int __init sstf_init(void)
{
        return elv_register(&elevator_sstf);
}

/* Taken from noop scheduler */
static void __exit sstf_exit(void)
{
        elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);

MODULE_AUTHOR("McKenna Jones");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSTF IO scheduler");
