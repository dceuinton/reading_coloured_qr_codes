#!/bin/bash

git add .
git status
echo -e "Enter a commit message: "
read message
git commit -m "$message"
