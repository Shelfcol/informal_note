#最开始的时候
git init #初始化仓库
git remote add origin git@github.com:Shelfcol/test3.git #添加远程仓库地址


git remote -v #查看地址
git add . #将当前修改添加至缓存区
git commit -m '1' #提交
git push #推送

#在github 创建仓库的时候，如果勾选添加readme文件后，在本地需要先执行 git pull 远程仓库地址 master ，确保远程仓库和本地一致,所以一般网站里面新建repo的时候不要勾选同时创建README.md

#后面使用的时候只需

#选做
git status #查看修改的文件
git diff #查看具体修改的内容

#必须做
git add . #将当前修改添加至缓存区
git commit -m '1' 
git push #推送
git push -u lab master#推送到lab上面，不是origin


#给repo加tag，则后面可以下载对应的tag
git add .
git commit -m 'gxf'
git tag -a v1.0 -m "version 1.0"
git push origin v1.0
