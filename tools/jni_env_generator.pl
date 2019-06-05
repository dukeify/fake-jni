#!/usr/bin/env perl

use warnings;
use strict;

my $file = "JNIEnv.txt";
open my ($env), $file;

while (my $proto = <$env>) {
    my ($i, $j, $k) = ($proto =~ /(\w+\*?)\s+\(\*(\w+)\)(.+);/);

    my $returnable = "return ";

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

#     my $output = <<'EOF';
# %s = []%s -> %s {
#  NativeInterface * const ni = (NativeInterface *)(env->functions);
# #ifdef FAKE_JNI_DEBUG
#  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::%s\n");
# #endif
#  return ni->%s();
# };
# EOF
#
#     # print(sprintf($output, $j, $k, $i, $j, $returnable));
#     print(sprintf($output, $j, $k, $i, $j, lcfirst($j)));

#     my $testing = <<'EOF';
# virtual %s %s%s const {
#  throw std::runtime_error("FATAL: '%s' is unimplemented.");
#  %s
# }
#
# EOF
    my $testing = <<'EOF';
virtual %s %s%s const = 0;
EOF


    # $k =~ s/JNIEnv *env\,//g;
    # $k =~ s/\(JNIEnv *env\)/()/g;

    # print(sprintf($testing, $i, lcfirst($j), $k, $j, $returnable))
    print(sprintf($testing, $i, lcfirst($j), $k))
}

close $env;
