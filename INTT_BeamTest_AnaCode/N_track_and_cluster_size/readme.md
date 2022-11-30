### The codes for the N of track and cluster size study of BeamTest2021
The analysis mainly focus on : 
1. run52 (production, beam spot in the middle)
2. run89 (the last production, beam spot at the edge)
3. run61 (vertical rotation)
4. run64 (with lead plate)

The MC and data comparison is also included.

The directory of data is "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/data/...". The results are also there.

The main functions are in the NTrack_ClusterSize.h

Note that : 
* for the MC file, the "track_single_cluster_v1.C" should be used to generate the cluster_information file
  * because the the ladder tilted problem doesn't need to be considered in MC 
* for the data file, the "track_single_cluster_v2.C".
  *the tilted of ladder along the longitudinal axis has to be taken into account.

