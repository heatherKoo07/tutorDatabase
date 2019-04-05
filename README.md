# tutorDatabase
tutorDatabase is a key-value storage engine developed by Heather Koo in De Anza college, and its main data structrue is two binary search trees and a hash table.

Currently in De Anza Collge, each department has their own system to manage student tutors. In addition, computer science department doesn't have a tool to manage tutoring. Students who look for a tutor needs to review paper printouts posted in the computer lab, and sign them in on paper every day. 
This application helps the students search tutor information easily by thier id and name. Furthermore, it helps the college have a centralized tutor database throughout all departmets. 

Tutor data has the following attributes - id(unique), name, subject, courses they have taken, available days, start time and end time. Each information is given from the file and the pointers of the objects are stored in two binary search trees and one hash table.

The first BST is sorted by the unique id, and the second one is sorted by name, which can be duplicate. The hash table uses id as a key and resolves collison with linked list. Also, hash function uses mid-square method. These data structures are implemented from scratch. 

This application allows the user search by id through hash table, and search by name through second BST. 
