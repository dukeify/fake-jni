#!/usr/bin/perl
use warnings;
use strict;

my $file = "JvmtiInterface.txt";
open my ($env), $file;

while (my $proto = <$env>) {
    my ($i, $j, $k) = ($proto =~ /(\w+\*?)\s+\(\*(\w+)\)(.+);/);
    if (!defined $i || !defined $j || !defined $k) { next; }
    #member stub replacement
#     my $output = <<'EOF';
# virtual %s %s%s const;
# EOF
#     print(sprintf($output, $i, lcfirst($j), $k));

    #constructor assignment replacement
    my $output = <<'EOF';
%s = []%s -> %s {
_FETCH_JVMTI_ENV
_JVMTI_DEBUG(%s)
 return ji->%s();
};
EOF

    print(sprintf($output, $j, $k, $i, $j, lcfirst($j)));
}

close $env;