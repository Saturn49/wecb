#include "act_mem_leak.h"

act_memleak_avl_node_t *efindNode(int key, act_memleak_avl_node_t* root) 
{
    act_memleak_avl_node_t* temp;
    ACT_VASSERT_RV(root != NULL, root);
    temp = root;
  
    while (temp !=NULL)
        {
            if (key == temp->key )
                return temp;
            else
                {
                    if (temp->key > key)
                        temp = temp->left;
                    else{
                        temp = temp->right;
                    }
                }
        }
    return NULL;
}





act_memleak_avl_node_t *findNode(int key, act_memleak_avl_node_t* root) 
{
    act_memleak_avl_node_t* temp;
    ACT_VASSERT_RV(root != NULL, root);
    temp = root;
    act_memleak_avl_node_t* find= NULL;
  
    while (temp !=NULL)
        {
            if (key == temp->key )
                return temp;
            else
                {
                    if (temp->key > key)
                        temp = temp->left;
                    else{
                        find = temp;
                        temp = temp->right;
                    }
                }
        }
    if (find == NULL || key - find->key > 0x3000){
        return NULL;
    }else {
        return find;
    }
  
}

int searchNode(int key, act_memleak_avl_node_t* root, act_memleak_avl_node_t** parent) 
{
    act_memleak_avl_node_t* temp;
    ACT_VASSERT_RV(root != NULL, 0);
    temp = root;
    *parent = root->parent;
    while (temp !=NULL)
        {
            if (temp->key == key)
                return 1;
            else
                {
                    *parent = temp;
                    if (temp->key > key)
                        temp = temp->left;
                    else
                        temp = temp->right;
                }
        }
    return 0;

}


act_memleak_avl_node_t* minNode(act_memleak_avl_node_t* root) //树root的最小结点
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

act_memleak_avl_node_t* maxNode(act_memleak_avl_node_t* root) //树root的最大结点
{
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

act_memleak_avl_node_t* preNode(act_memleak_avl_node_t* target) //求前驱结点
{
    if (target == NULL)
        return NULL;
    if (target->left != NULL)
        return maxNode(target->left);
    else
        while ((target->parent!=NULL) && (target!=target->parent->right))
            target = target->parent;
    return target->parent;
}

act_memleak_avl_node_t* nextNode(act_memleak_avl_node_t* target) //求后继结点
{
    if (target == NULL)
        return NULL;
    if (target->right != NULL)
        return minNode(target->right);
    else
        while ((target->parent!=NULL) && (target!=target->parent->left))
            target = target->parent;
    return target->parent;
}

act_memleak_avl_node_t* adjustAVL(act_memleak_avl_node_t* root, act_memleak_avl_node_t* parent, act_memleak_avl_node_t* child)
{
    act_memleak_avl_node_t *cur;
    ACT_VASSERT_RV((parent != NULL)&&(child != NULL), root);
    switch (parent->balance)
        {
        case 2:
            if (child->balance == -1)//LR型
                {
                    cur = child->right;
                    cur->parent = parent->parent;
                    child->right = cur->left;
                    if (cur->left != NULL)
                        cur->left->parent = child;
                    parent->left = cur->right;
                    if (cur->right != NULL)
                        cur->right->parent = parent;
                    cur->left = child;
                    child->parent = cur;
                    cur->right = parent;
                    if (parent->parent != NULL)
                        if (parent->parent->left == parent)
                            parent->parent->left = cur;
                        else parent->parent->right = cur;
                    else
                        root = cur;
                    parent->parent = cur;
                    if (cur->balance == 0)
                        {
                            parent->balance = 0;
                            child->balance = 0;
                        }
                    else if (cur->balance == -1)
                        {
                            parent->balance = 0;
                            child->balance = 1;
                        }
                    else
                        {
                            parent->balance = -1;
                            child->balance = 0;
                        }
                    cur->balance = 0;
                }
            else //LL型
                {
                    child->parent = parent->parent;
                    parent->left = child->right;
                    if (child->right != NULL)
                        child->right->parent = parent;
                    child->right = parent;
                    if (parent->parent != NULL)
                        if (parent->parent->left == parent)
                            parent->parent->left = child;
                        else parent->parent->right = child;
                    else
                        root = child;
                    parent->parent = child;
                    if (child->balance == 1) //插入时
                        {
                            child->balance = 0;
                            parent->balance = 0;
                        }
                    else //删除时
                        {
                            child->balance = -1;
                            parent->balance = 1;
                        }
                }
            break;
   
        case -2:
            if (child->balance == 1) //RL型
                {
                    cur = child->left;
                    cur->parent = parent->parent;
                    child->left = cur->right;
                    if (cur->right != NULL)
                        cur->right->parent = child;
                    parent->right = cur->left;
                    if (cur->left != NULL)
                        cur->left->parent = parent;
                    cur->left = parent;
                    cur->right = child;
                    child->parent = cur;
                    if (parent->parent != NULL)
                        if (parent->parent->left == parent)
                            parent->parent->left = cur;
                        else parent->parent->right = cur;
                    else
                        root = cur;
                    parent->parent = cur;
                    if (cur->balance == 0)
                        {
                            parent->balance = 0;
                            child->balance = 0;
                        }
                    else if (cur->balance == 1)
                        {
                            parent->balance = 0;
                            child->balance = -1;
                        }
                    else
                        {
                            parent->balance = 1;
                            child->balance = 0;
                        }
                    cur->balance = 0;
                }
            else  //RR型
                {
                    child->parent = parent->parent;
                    parent->right = child->left;
                    if (child->left != NULL)
                        child->left->parent = parent;
                    child->left = parent;
                    if (parent->parent != NULL)
                        if (parent->parent->left == parent)
                            parent->parent->left = child;
                        else parent->parent->right = child;
                    else
                        root = child;
                    parent->parent = child;
                    if (child->balance == -1) //插入时
                        {
                            child->balance = 0;
                            parent->balance = 0;
                        }
                    else //删除时
                        {
                            child->balance = 1;
                            parent->balance = -1;
                        }
                }
            break;
        }
    return root;
}


act_memleak_avl_node_t* insertNode(int key, void *p_value, act_memleak_avl_node_t* root)
{
    act_memleak_avl_node_t *parent, *cur, *child;
    ACT_VASSERT_RV (root != NULL, root);
    if (searchNode(key, root, &parent)){ //结点已存在
        return root;
    }
    else
        {
            ACT_MALLOC_RV(cur, act_memleak_avl_node_t, root);
            cur->parent = parent;
            cur->key = key;
            cur->p_value = p_value;
            cur->left = NULL;
            cur->right = NULL;
            cur->balance = 0;
            if (key<parent->key)
                {
                    parent->left = cur;
                    child = parent->left;
                }
            else
                {
                    parent->right = cur;
                    child = parent->right;
                }
   
            while ((parent != NULL)) //查找需要调整的最小子树
                {
                    if (child == parent->left)
                        if (parent->balance == -1)
                            {
                                parent->balance = 0;
                                return root;
                            }
                        else if (parent->balance == 1)
                            {
                                parent->balance = 2;
                                break;
                            }
                        else
                            {
                                parent->balance = 1;
                                child = parent;
                                parent = parent->parent;
                            }
                    else if (parent->balance == 1)
                        {
                            parent->balance = 0;
                            return root;
                        }
                    else if (parent->balance == -1)
                        {
                            parent->balance = -2;
                            break;
                        }
                    else
                        {
                            parent->balance = -1;
                            child = parent;
                            parent = parent->parent;
                        }
                }
   
            if (parent == NULL)
                return root;
            return adjustAVL(root, parent, child);
        }
}


act_memleak_avl_node_t* deleteNode(int key, act_memleak_avl_node_t* root)
{
    act_memleak_avl_node_t *dNode, *parent, *child;
    int temp;
    int temp_val;
    ACT_VASSERT_RV(root!=NULL, root);
    if (!searchNode(key, root, &parent))
        return root;
    else
        {
            if (parent == NULL)
                dNode = root;
            else if ((parent->left!=NULL)&&(parent->left->key==key))
                dNode = parent->left;
            else dNode = parent->right;

            child = dNode;
            while ((child->left!=NULL)||(child->right!=NULL)) //确定需要删除的结点
                {
                    if (child->balance == 1)
                        child = preNode(dNode);
                    else 
                        child = nextNode(dNode);
                    
                    temp = child->key;
                    temp_val = (int)child->p_value;
                    
                    child->key = dNode->key;
                    child->p_value = dNode->p_value;
                    
                    dNode->key = temp;
                    dNode->p_value = (void *)temp_val;
                    dNode = child;
                }
   
            child = dNode;
            parent = dNode->parent;
      
            while ((parent != NULL)) //查找需要调整的最小子树
                {
                    if (child == parent->left)
                        if (parent->balance == 1)
                            {
                                parent->balance = 0;
                                child = parent;
                                parent = parent->parent;
                            }
                        else if (parent->balance == -1)
                            {
                                parent->balance = -2;
                                child = parent->right;
                                break;
                            }
                        else
                            {
                                parent->balance = -1;
                                break;
                            }
                    else if (parent->balance == -1)
                        {
                            parent->balance = 0;
                            child = parent;
                            parent = parent->parent;
                        }
                    else if (parent->balance == 1)
                        {
                            parent->balance = 2;
                            child = parent->left;
                            break;
                        }
                    else
                        {
                            parent->balance = 1;
                            break;
                        }
                }
            if (dNode->parent != NULL){
                if (dNode == dNode->parent->left)
                    dNode->parent->left = NULL;
                else dNode->parent->right = NULL;
            }
            if (dNode->p_value!= NULL){
                ACT_FREE(dNode->p_value);
                dNode->p_value = NULL;
            }
    
            ACT_FREE(dNode);
            if (root == dNode)
                root = NULL; //root被删除, 避免野指针
            dNode = NULL;
            if ((parent==NULL) || (parent->balance==1) || (parent->balance==-1))
                return root;
            return adjustAVL(root, parent, child);
        }
}


void destroyAVL(act_memleak_avl_node_t* root)
{
    if (root != NULL)
        {
            destroyAVL(root->left);
            destroyAVL(root->right);
    
            if (root->p_value!= NULL){
                ACT_FREE(root->p_value);
                root->p_value = NULL;
            }
            ACT_FREE(root);
            root=NULL;
        }
  
}



void traverseAVL1(act_memleak_avl_node_t* root) //中序遍历
{
    if (root != NULL)
        {
            traverseAVL1(root->left);
            printf("0x%x, %d %d \n", root->key, root->balance, root->p_value);
            traverseAVL1(root->right);
        }
}


