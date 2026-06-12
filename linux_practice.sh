#!/bin/bash
mkdir linux_practice
mkdir linux_practice/docs
mkdir linux_practice/backup
touch linux_practice/docs/{readme.txt,notes.log,temp.tmp}
rm linux_practice/docs/temp.tmp
mv linux_practice/docs/notes.log linux_practice/docs/daily_report.txt
echo "Project Status: Active" > linux_practice/docs/daily_report.txt
echo date >> linux_practice/docs/dail_report.txt
cp linux_practice/docs/*.txt linux_practice/backup/
chmod 444 linux_practice/backup/*
echo "Archive Complete.File readme.txt is now read-only"
echo "Archive Complete.File dailu_report.txt is now read-only"
