# Contribution guidelines

You're welcome to contribute to this project! There are several aspects you can help on:

- improving our documentation (see [doc/readme.txt](doc/readme.txt) file and [doc](doc) folder),
- translations,
- testing preview releases,
- helping other users on the mailing lists,
- improving our UIs, or also
- coding (e.g., fix open issues or implement new features; see below for more information).
  - Building TortoiseGit normally is not necessary, however, it is easy. All necessary requirements and steps are described in the [build.txt](build.txt) file. Helpful might also be our short description in the [architecture.txt](architecture.txt) file.

Any help is appreciated!

Please also see https://tortoisegit.org/contribute/!

## Short version for the impatient

### Submission:

- open an issue on https://tortoisegit.org/issues or go to an
  issue you want to fix. Then you have the following options:
  - open a GitLab merge request and put the link into the issue
  - open a GitHub pull request and put the link into the issue
  - attach a patch there
  - provide a link to a branch you want the maintainers to pull from

For existing issues it is recommended to discuss possible solutions on the issue
before you start coding.

### Commits:

- make commits of (small) logical units
- do not check in commented out code, unrelated changes or unneeded files
- the subject (first line of the commit message) should be a short
  description and should skip the full stop. If it is about a bug 
  report/issue, use `Fixed issue #NUMBER: ISSUE'S TITLE`
- the second line of commit message should be an empty line
- the body which starts from the third line should provide a meaningful
  commit message
- consier writing a unit test
- add a `Signed-off-by: Your Name <you@example.com>` line to the commit
  message to confirm that you agree to the Developer's Certificate of
  Origin.
  Please use your real name.

### Coding style:

- based on Visual Studio default coding style (try to copy & paste)
- the old code migrates to new coding style only when modifying it.
- variable definitions (as it is a type definition; e.g., CGitHash* name 
  and CGitHash& name)
- for `if`, `for`, `while` a spaces between the keyword and the 
  parenthesis (e.g., if ()).
- braces go on the next line for `if`, `for`, `else`, ...
- If an `if` contains only one action, no braces needed.
- function calls, there is no space before the parenthesis (e.g, method()).
- no spaces before and after parentheses (e.g., (something)).
- spaces between &&, + and other operands also after comma(,).
- no variable shadowing.
- for variable names: we don't have a strict rule here, should be speak 
  for it self.
- try not to nest too deeply and return as early as possible.
- Only use `this` if needed.

## Longer version:

### (0) Make separate commits for logically separate changes.

Unless your patch is really trivial, you should not be sending
out a patch that was generated between your working tree and
your commit head. Instead, always make a commit with complete
commit message and generate a series of patches from your
repository.

Give an explanation for the change(s) that is detailed enough so
that people can judge if it is good thing to do, without reading
the actual patch text to determine how well the code does what
the explanation promises to do.

If your description starts to get too long, that's a sign that you
probably need to split up your commit to finer grained pieces.
That being said, patches which plainly describe the things that
help reviewers check the patch, and future maintainers understand
the code, are the most beautiful patches. Descriptions that summarise
the point in the subject well, and describe the motivation for the
change, the approach taken by the change, and if relevant how this
differs substantially from the prior version, are all good things
to have.

Oh, another thing. We are picky about whitespaces. To help ensure this
does not happen, run `git diff --check` on your changes before you commit.

### (1a) Create a merge/pull request

Push your changes to a public repository. Use a brief branch name
describing your changes.

Please make sure your pull request does not include any extra files
which do not belong in a submission. Make sure to review your patch
after generating it, to ensure accuracy.

Create a GitLab merge request (https://gitlab.com/tortoisegit/tortoisegit/),
a GitHub pull request (https://github.com/TortoiseGit), or use the URL and
branchname of the changes in your repository.

### (1b) Generate your patch using git tools out of your commits (Patch serial)

git based diff tools generate unified diff which is the preferred format.

Please make sure your patch does not include any extra files
which do not belong in a patch submission. Make sure to review
your patch after generating it, to ensure accuracy. Before
sending out, please make sure it cleanly applies to the "master"
branch head.

### (2) Sign your work

To improve tracking of who did what, we've borrowed the
"sign-off" procedure from the Linux kernel project on patches
that are being emailed around. Although TortoiseGit is a lot
smaller project it is a good discipline to follow it.

The sign-off is a simple line at the end of the explanation for
the patch, which certifies that you wrote it or otherwise have
the right to pass it on as a open-source patch. The rules are
pretty simple: if you can certify the below:

	Developer's Certificate of Origin 1.1

	By making a contribution to this project, I certify that:

	(a) The contribution was created in whole or in part by me and I
	    have the right to submit it under the open source license
	    indicated in the file; or

	(b) The contribution is based upon previous work that, to the best
	    of my knowledge, is covered under an appropriate open source
	    license and I have the right under that license to submit that
	    work with modifications, whether created in whole or in part
	    by me, under the same open source license (unless I am
	    permitted to submit under a different license), as indicated
	    in the file; or

	(c) The contribution was provided directly to me by some other
	    person who certified (a), (b) or (c) and I have not modified
	    it.

	(d) I understand and agree that this project and the contribution
	    are public and that a record of the contribution (including all
	    personal information I submit with it, including my sign-off) is
	    maintained indefinitely and may be redistributed consistent with
	    this project or the open source license(s) involved.

then you just add a line saying

	Signed-off-by: Random J Developer <random@developer.example.org>

This line can be automatically added by Git and TortoiseGit.

Also notice that a real name is used in the Signed-off-by: line. Please
don't hide your real name.

If you like, you can put extra tags at the end:

1. "Reported-by:" is used to credit someone who found the bug that
   the patch attempts to fix.
2. "Acked-by:" says that the person who is more familiar with the area
   the patch attempts to modify liked the patch.
3. "Reviewed-by:", unlike the other tags, can only be offered by the
   reviewer and means that s/he is completely satisfied that the patch
   is ready for application. It is usually offered only after a
   detailed review.
4. "Tested-by:" is used to indicate that the person applied the patch
   and found it to have the desired effect.

You can also create your own tag or use one that's in common usage
such as "Thanks-to:", "Based-on-patch-by:", or "Mentored-by:".

## An ideal patch flow

Here is an ideal patch flow for this project the current maintainer
suggests to the contributors:

0. You come up with an itch. You code it up.
   For existing issues it is recommended to discuss possible solutions on
   the issue before you start coding.

1. Open an issue on https://tortoisegit.org/issues or create a merge/pull
   request or attach the patches to the issue there.

2. You get comments and suggestions for improvements.

3. Polish, refine, and update/re-send. Go back to step (2).

4. The patch will be applied to `master`.
