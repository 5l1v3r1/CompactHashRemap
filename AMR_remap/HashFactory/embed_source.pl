eval 'exec perl -w $0 ${1+"$@"}'
  if 0;

# Copyright 2013-19.  Triad National Security, LLC. This material was produced
# under U.S. Government contract 89233218CNA000001 for Los Alamos National 
# Laboratory (LANL), which is operated by Triad National Security, LLC
# for the U.S. Department of Energy. The U.S. Government has rights to use,
# reproduce, and distribute this software.  NEITHER THE GOVERNMENT NOR
# TRIAD NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
# ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified
# to produce derivative works, such modified software should be clearly marked,
# so as not to confuse it with the version available from LANL.   
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy
# of the License at 
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.
#
# Under this license, it is required to include a reference to this work. We
# request that each derivative work contain a reference to LANL Copyright 
# Disclosure C14043/LA-CC-14-003 so that this work's impact can be roughly
# measured.
#
# This is LANL Copyright Disclosure C14043/LA-CC-14-003
#

use File::Basename;

$ARGV_LAST = "";

($file,,) = fileparse($ARGV[0], qr/\..*/);
print "const char *",$file,"_source =\n";

while(<>) {
   if ($ARGV ne $ARGV_LAST){
#      $comment = 0;
      $ARGV_LAST=$ARGV;
   }
   chop $_;

   s/"/""/g;

#   if (/^\/\*/){
#      #print "Setting comment to 1\n";
#      $comment = 1;
#   }
#   if (/\*\/$/){
#      #print "Setting comment to 0\n";
#      $comment = 0;
#      next;
#   }
#   if ($comment eq 1) {
#      #print "Skip line and goto next\n";
##      next;
#   }

   print '"',$_,'\n','"',"\n";
}

print ";\n"
