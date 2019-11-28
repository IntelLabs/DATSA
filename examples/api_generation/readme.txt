This data generator is used to create textual representations of programs 
consisting of:

  - 1 to N API calls (e.g., insert, remove, find)
  - 1 associated value with each API call
  - The entire program is stored in one line

-----------------------------------------------------------------------
Example:

(Assume 1 = insert, 2 = find, 3 = remove)

Contents of dataset file:

1 1 872 1 9 1 28 2 22 3 9
2 1 7 2 7 1 28 2 0 3 7 3 28
EOF

This dataset translates to the following two programs:

Program 1:

insert(872)
insert(9)
insert(28)
find(22)
remove(9)

Program 2:

insert(7)
find(7)
insert(28)
find(0)
remove(7)
remove(28)

-----------------------------------------------------------------------

The data generator creates up to M programs, where M is nearly unbounded 
in size (up to more than 1,000,000,000 unique programs).

The data generator also __guarantees__ no two programs generated are identical 
using DATSA's hard uniqueness property. See DATSA's tutorial for more 
information. The generator will terminate if it cannot generate uniqueness.

As such, be sure that the combination of APIs and variable value ranges of you
supply are diverse enough to generate the total number of programs you are
aiming to generate.

You can disable the hard uniqueness requirement in the code by either 
commenting out that line or changing it to "soft" uniqueness.

Questions: justin.gottschlich@intel.com

