UPDATE creature_template SET ScriptName='npc_shade_of_hakkar' WHERE entry=8440;
UPDATE creature_template SET ScriptName='npc_hakkar_suppressor' WHERE entry=8497;
UPDATE gameobject_template SET ScriptName='go_eternal_flame' WHERE entry IN (148418,148419,148420,148421);

DELETE FROM scripted_event_id WHERE id IN (3094,3095,3097,3098,3099,3100, 8502);
INSERT INTO scripted_event_id VALUES
(3094,'event_antalarion_statue_activation'),
(3095,'event_antalarion_statue_activation'),
(3097,'event_antalarion_statue_activation'),
(3098,'event_antalarion_statue_activation'),
(3099,'event_antalarion_statue_activation'),
(3100,'event_antalarion_statue_activation'),
(8502,'event_avatar_of_hakkar');
