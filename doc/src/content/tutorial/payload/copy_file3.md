+++
title = "Auto-throwing filesystem_error"
description = ""
weight = 30
+++

Something not mentioned at all until now (and properly described in the next
section, [Default actions](../default-actions/)) is that Outcome can be
programmed take various actions when the user tries to observe `.value()`
when there is no value, and so on for the other possible state observations.

Seeing as we are replacing the throwing overload of `copy_file()` in the
Filesystem TS with a `result` returning edition instead, it would make
sense if an attempt to observe the value of an unsuccessful `fs_result`
threw the exact same `filesystem_error` as the Filesystem TS does.

Telling Outcome how to throw a `filesystem_error` with payload of the
failing paths is easy:

{{% snippet "outcome_payload.cpp" "filesystem_api_custom_throw" %}}

Usage of our new "upgraded" Filesystem `copy_file()` might now be as follows:

{{% snippet "outcome_payload.cpp" "filesystem_api_custom_throw_demo" %}}

