#
# merge.sh: Merge and delete dev branch.
# Switches to master to merge changes from dev branch and deletes it afterwards.
#
git checkout master && git merge dev && git branch -d dev