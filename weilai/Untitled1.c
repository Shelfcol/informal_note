//链表的倒数第K个值
struct listNode{
    int val;
    ListNode* next;
    ListNode(int _val,ListNode* _next=NULL):val(_val),next(_next){}
};

ListNode* kNumberOfLsit(listNode* head,int k)
{
    if(!head||k<=0) return nullptr;//
    ListNode* fast=head;
    ListNode* slow=head;

    while(k>0&&fast!=nullptr)
    {
        fast=fast->next;
        --k;
    }
    if(k>0) return nullptr;//表示没有k个节点
    while(fast!=nullptr)
    {
        slow=slow->next;
        fast=fast->next;
    }
    return slow;
}

struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    int val;
    TreeNode(TreeNode* _left=NULL,TreeNode* _right=NULL,int _val=0):left(_left),right(_right),val(_val){}
};

vector<int> queTraverse(TreeNode* root)
{
    vector<int> ans;
    if(!root)   return ans;
    queue<TreeNode*> que;
    que.push(root);
    while(que.size()!=0)
    {
        int que_size=que.size();
        for(int i=0;i<que_size;++i)
        {
            TreeNode* tmp=que.front();
            que.pop();
            ans.push_back(tmp->val);
            if(tmp->left)   que.push(tmp->left);
            if(tmp->right)  que.push(tmp->right);
        }
    }
    return ans;
}
