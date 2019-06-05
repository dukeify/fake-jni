#!/usr/bin/env perl

use warnings;
use strict;

my $file = "NativeInterface.txt";
open my ($env), $file;

sub stubbed_ret_type {
    my ($i) = @_;

    my $returnable = "\n    return ";

    if ($i eq "void") { $returnable = ""; }
    elsif ($i eq "jint") { $returnable .= "0"; }
    elsif ($i eq "jclass") { $returnable .= "nullptr"; }
    elsif ($i eq "jmethodID") { $returnable .= "0"; }
    elsif ($i eq "jfieldID") { $returnable .= "0"; }
    elsif ($i eq "jobject") { $returnable .= "nullptr"; }
    elsif ($i eq "jboolean") { $returnable .= "0"; }
    elsif ($i eq "jthrowable") { $returnable .= "nullptr"; }
    elsif ($i eq "jbyte") { $returnable .= "0"; }
    elsif ($i eq "jchar") { $returnable .= "0"; }
    elsif ($i eq "jshort") { $returnable .= "0"; }
    elsif ($i eq "jlong") { $returnable .= "0"; }
    elsif ($i eq "jfloat") { $returnable .= "0.0f"; }
    elsif ($i eq "jdouble") { $returnable .= "0.0"; }
    elsif ($i eq "jsize") { $returnable .= "0"; }
    elsif ($i eq "jstring") { $returnable .= "nullptr"; }
    elsif ($i eq "jchar*") { $returnable .= "nullptr"; }
    elsif ($i eq "char*") { $returnable .= "nullptr"; }
    elsif ($i eq "jobjectArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jbooleanArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jbyteArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jcharArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jshortArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jintArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jlongArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jfloatArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jdoubleArray") { $returnable .= "nullptr"; }
    elsif ($i eq "jboolean*") { $returnable .= "nullptr"; }
    elsif ($i eq "jbyte*") { $returnable .= "nullptr"; }
    elsif ($i eq "jshort*") { $returnable .= "nullptr"; }
    elsif ($i eq "jint*") { $returnable .= "nullptr"; }
    elsif ($i eq "jlong*") { $returnable .= "nullptr"; }
    elsif ($i eq "jfloat*") { $returnable .= "nullptr"; }
    elsif ($i eq "jdouble*") { $returnable .= "nullptr"; }
    elsif ($i eq "void*") { $returnable .= "nullptr"; }
    elsif ($i eq "jweak") { $returnable .= "nullptr"; }
    elsif ($i eq "jobjectRefType") { $returnable .= "nullptr"; }

    else { die "unhandled type: " . $i . "\n"; }

    if ($i ne "void") { $returnable .= ";" }

    return $returnable;
}

while (my $proto = <$env>) {
    my ($i, $j, $k) = ($proto =~ /virtual (\w+\s?\*?)(\w+)(\(.+\)?) const;/);

    if (!defined $i || !defined $j || !defined $k) { next; }

    my $output = <<'EOF';
//TODO implement
%s NativeInterface::%s%s const {
    throw std::runtime_error("FATAL: 'JVMNativeInterface_::%s' is unimplemented!");%s
}
EOF

    $i =~ s/\s+//g;
    print(sprintf($output, $i, $j, $k, $j, stubbed_ret_type($i)) . "\n");
}