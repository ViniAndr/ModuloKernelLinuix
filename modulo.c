#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROC_NAME "<VINICIUS_DEV>_PROC_FS"

static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
    static unsigned long counter = 0;

    if (*pos == 0) {
        return &counter;
    }

    *pos = 0;
    return NULL;
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    unsigned long *tmp_v = (unsigned long *)v;
    (*tmp_v)++;
    (*pos)++;
    return NULL;
}

static void my_seq_stop(struct seq_file *s, void *v)
{
}

static int proc_show(struct seq_file *s, void *v)
{
    loff_t *spos = (loff_t *)v;
    seq_printf(s, "%Ld\n", *spos);
    return 0;
}

static struct seq_operations my_seq_ops = {
    .start = my_seq_start,
    .next = my_seq_next,
    .stop = my_seq_stop,
    .show = proc_show,
};

static int my_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &my_seq_ops);
}

#ifdef HAVE_PROC_OPS

static const struct proc_ops proc_fops = {
    .proc_open = my_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = seq_release,
};

#else

static const struct file_operations proc_fops = {
    .open = my_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};

#endif

static int __init porc_init(void)
{
    struct proc_dir_entry *entry;
    entry = proc_create(PROC_NAME, 0, NULL, &proc_fops);

    if (entry == NULL) {
        remove_proc_entry(PROC_NAME, NULL);
        pr_debug("Erro: Não foi possível inicializar /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_debug("Processo %s adicionado com sucesso!\n", PROC_NAME);
    return 0;
}

static void __exit proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_debug("/proc/%s removido\n", PROC_NAME);
    pr_debug("VOCE PASSOU!\n");
}

module_init(porc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FABIO VINICIUS BARBOSA DE ANDRADE");

