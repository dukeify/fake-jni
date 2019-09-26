#!/usr/bin/perl
use warnings;
use strict;

my $file = "JvmtiStubs.txt";
open my ($env), $file;

while (my $proto = <$env>) {
    my ($i, $j, $k) = ($proto =~ /virtual (\w+\s?\*?)(\w+)(\(.+\)?) const;/);
    if (!defined $i || !defined $j || !defined $k) { next; }
    # implementation replacement
    my $output = <<'EOF';
//TODO implement
%sJvmtiInterface::%s%s const {
    throw std::runtime_error("FATAL: 'jvmtiInterface_1_::%s' is unimplemented!");
}
EOF
    print(sprintf($output, $i, lcfirst($j), $k, $j) . "\n");
}

close $env;