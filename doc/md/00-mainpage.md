<center><table border="0" cellpadding="4">
<tr>
<td align="center"> <a href="https://dedi4.nedprod.com/static/files/boost.outcome-v1.0-source-latest.tar.xz">Source</a><br><a href="https://dedi4.nedprod.com/static/files/boost.outcome-v1.0-source-latest.tar.xz">Tarball</a> </td>
<td align="center"> <a href="https://github.com/ned14/boost.outcome">Boost.Outcome</a><br><a href="https://github.com/ned14/boost.outcome">on GitHub</a> </td>
<td align="center"> <a href="http://my.cdash.org/index.php?project=Boost.Outcome">CTest summary</a><br><a href="http://my.cdash.org/index.php?project=Boost.Outcome">dashboard</a> </td>
<td align="center"> <a href="https://travis-ci.org/ned14/boost.outcome">Linux and MacOS CI:</a><img src="https://travis-ci.org/ned14/boost.outcome.svg?branch=master"/> </td>
<td align="center"> <a href="https://ci.appveyor.com/project/ned14/boost-outcome/branch/master">Windows CI:</a><img src="https://ci.appveyor.com/api/projects/status/roe4dacos4gnlu66/branch/master?svg=true"/> </td>
</tr>
</table></center>

[TOC]

\section description Description

This is the Outcome library. It is a C++ 14 library intended to aid ultra-lightweight
error handling in large C++ codebases, providing a more expressive and type safe
alternative to integer error codes or enums.

Unlike alternative implementations, it works perfectly with exceptions and RTTI
disabled and is thus suitable for low-latency/games/finance/SG14 users. One could
view Outcome as a minimum overhead universal outcome transport mechanism for C++,
hence being named "Outcome".

The Outcome library provides an implementation of `expected<T, E>` (which is on the
C++ 20 standardisation track), with the `expected<T, E>` refinements `outcome<T>` and
`result<T>`.

If you are familiar with Swift's error code throws or Rust's `Result<T, E>`,
you will find almost identical semantics in the transports provided here.
Outcome even has a \ref try_operation "BOOST_OUTCOME_TRY" macro doing most of the
`try` keyword in Rust and Swift!

\note Outcome has not been peer reviewed and is not part of the Boost libraries
(yet). It will undergo its first round of peer review end of May 2017.

One motivation for this library is to manage errors reported by different means
using a single \ref performance_overhead "minimum overhead" framework. This
motivating example should clarify:

\snippet simple_example.cpp simple_example


\page prerequisites Prerequisites

Outcome is a header only C++ 14 library known to work on these compiler-platform combinations or better:
- clang 3.5 (LLVM) [FreeBSD, Linux, OS X]
- clang 3.7 (with Microsoft Codegen) [Windows]
- GCC 5.4 [Linux]
- VS2015 Update 2 [Windows]
- Xcode 7.3 [OS X]

**A copy of Boost is not required to use this library**. You can simply drop Outcome into
your project and go.

\warning MSVC generates significant code bloat when using Outcome in large code bases.
If you can use VS2017 which implements C++ 14 constexpr and has a better optimiser for
modern C++, you will see tighter executables. Execution speed is not particularly
different, though one would have thought the extra cache load caused by code bloat might
affect some applications. In this situation, use LLVM clang targeting the MSVC ABI.


\page support Support

<a href="https://stackoverflow.com/">StackOverflow</a> with the <tt>boost-outcome</tt> tag is the
preferred place to ask questions on usage.

If you have a problem, please review the FAQ and the wiki. Searching
<a href="https://github.com/ned14/boost.outcome/issues">the issues</a>
for your problem is also a good idea. If you are encountering what you
think is a bug, please open an issue.
