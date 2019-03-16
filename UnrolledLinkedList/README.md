# TASK - Unrolled Linked List

## Definition
In computer programming, an unrolled linked list is a variation on the linked list which stores multiple elements in each node. It can dramatically increase cache performance, while decreasing the memory overhead associated with storing list metadata such as references. It is related to the B-tree.


## Methods 
*push_back - Adds element in the end of a list
*push_to_node - Adds an element to a specific node
*delete_occurances - Removes the occurances of an element from all nodes
*empty - Returns is the list is empty
*size - Returns the size of the list

## Iterator
operator++ (Prefix and Postfix) - the iterator moves element by element, reaching the end of a node, passing to the next node
operator* - Returns the value of the iterator
operator+ - Moves the iterator with as many elements as a parameter
operator== and operator-- - Returns if 2 iterators point at the same things 
