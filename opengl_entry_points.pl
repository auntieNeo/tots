#!/usr/bin/env perl
use warnings;
use strict;

my $procs = [];
while(<STDIN>) {
  chomp;
  push(@$procs, $_);
}
foreach(@$procs) {
  printf("GL_EXTERN PFN%sPROC %s;\n", uc $_, $_);
}
foreach(@$procs) {
  printf("  %s = (PFN%sPROC)SDL_GL_GetProcAddress(\"%s\");\n", $_, uc $_, $_);
}
