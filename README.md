# Lockheed Martin - 3 ( Packer / Loader )

| Faculty Mentor              | Customer POC & SME |
| :-------------------------: |:------------------:|
| [Sabetto](mailto:rsabetto@gmu.edu) | [Matt Murray](mailto:matthew.m.murray@lmco.com), [Steve Kyle](mailto:steven.t.kyle@lmco.com), [Austin Keeley](mailto:austin.t.keeley@lmco.com) |

## Questions

1. Do they want the packer to send the loader along with the file?

2. What encryption standard do you need us to meet? (We're thinking AES)

3. Can we use 7zip? Has native encryption, multithreading, etc.

4. To our understanding, this is not asking for a packer but a secure distribution method.

5. "Packer must be able to support dynamic linking against libraries" and "Packer supports both static and dynamic linking against libraries" can you clarify what you mean by this? (included in ingest, or have to crawl across executable?)

6. Can you please outline an expected use case?


## Notes

 - PE file is not going to run in 
 
 - Remove zip header?  Compress first, then run every byte through AES...
 
 - Loader never touches disk.  This is tougher, but doable for us. We will try this last.
